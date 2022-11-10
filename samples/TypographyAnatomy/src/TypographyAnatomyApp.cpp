#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/SdfText.h"
#include "cinder/Utilities.h"
#include "cinder/CinderImGui.h"
#include "cinder/Log.h"

using namespace ci;
using namespace ci::app;
using namespace std;

struct FontInfo {
    std::string mName;
    std::vector<gl::SdfText::Font> mFonts;
    std::vector<gl::SdfTextRef> mSdfTexts;

    FontInfo() {}
    FontInfo(const std::string& name)
        : mName(name) {}
};

class TypographyAnatomyApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;

	std::vector<FontInfo> mFonts;
    std::vector<int> mFontSizes = {38, 50, 67, 89};
    std::vector<std::string> mSampleText;
    std::string mSampleParagraph;
    int mFontSelect;
    int mSizeSelect;
    int mAlignmentSelect;
    int mVerticalAlignmentSelect;
    float mTracking;
    float mLeading;

    bool mDrawBoundingBoxes;
    bool mDrawFitRects;
    bool mDrawAscenderLine;
    bool mDrawDescenderLine;

    std::vector<std::string> mAlignmentLabels = {"Left", "Center", "Right"};
    std::vector<std::string> mVerticalAlignmentLabels = {"Top", "Middle", "Bottom"};
    bool mJustify = false;
    bool mPremultiply = false;
    bool mLigate = false;
    bool mUseHalfLeading = false;
};

void TypographyAnatomyApp::setup() {
#if defined(CINDER_MSW)
    // For AllSamples
    addAssetDirectory(getAppPath() / "../../../../TypographyAnatomy/assets");
#endif

    mFonts.push_back(FontInfo("Arial"));
    mFonts.push_back(FontInfo("Calibri"));
    mFonts.push_back(FontInfo("Cambria"));
    mFonts.push_back(FontInfo("Franklin Gothic"));
    mFonts.push_back(FontInfo("Futura"));
    mFonts.push_back(FontInfo("Rockwell Nova"));
    mFonts.push_back(FontInfo("Times New Roman"));
    mFonts.push_back(FontInfo("Verdana"));

    mSampleText.push_back("The quick brown fox jumps over the lazy dog.");
    //mSampleText.push_back("A very bad quack might jinx zippy fowls.");
    //mSampleText.push_back("The five boxing wizards jumped quickly.");
    //mSampleText.push_back("Pack my box with five dozen liquor jugs.");
    //mSampleText.push_back("Fix problems quickly with galvanized jets.");
    mSampleText.push_back("Ligatures: fb ff fi fj fk fl ft ffb ffi ffj ffk ffl fft Th");

    mSampleParagraph =
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nullam in velit vehicula, porta ligula ut, luctus "
        "justo. Aliquam vitae ullamcorper odio. Nullam placerat erat nunc, id faucibus nisl pretium eget. Integer a ";

    mFontSelect = 0;
    mSizeSelect = 0;
    mAlignmentSelect = 0;
    mVerticalAlignmentSelect = 0;
    mDrawBoundingBoxes = false;
    mDrawFitRects = false;
    mDrawAscenderLine = false;
    mDrawDescenderLine = false;
    mTracking = 0.0f;
    mLeading = 1.0f;

    for (auto& font : mFonts) {
        for (auto& size : mFontSizes) {
            auto f = gl::SdfText::Font(font.mName, size);
            font.mFonts.push_back(f);
            std::string sdft_filename = ci::app::getAssetPath("").string() + "cached_fonts/" + font.mName + "_" + toString(size) + ".sdft";
            auto s = gl::SdfText::create(sdft_filename, f, gl::SdfText::Format());
            font.mSdfTexts.push_back(s);
        }
    }

    ImGui::Initialize();
}

void TypographyAnatomyApp::mouseDown( MouseEvent event ) {
}

void TypographyAnatomyApp::update() {
    ImGui::Begin("Anatomy of a Font");
    bool isChanged;
    if (ImGui::CollapsingHeader("Font Options", ImGuiTreeNodeFlags_DefaultOpen)) {
        if (ImGui::ListBoxHeader("Selected Font")) {
            int counter = 0;
            for (auto& item : mFonts) {
                if (ImGui::Selectable(item.mName.c_str(), counter == mFontSelect)) {
                    mFontSelect = counter;
                }
                counter++;
            }
            ImGui::ListBoxFooter();
        }
        if (ImGui::ListBoxHeader("Selected Font Size")) {
            int counter = 0;
            for (auto& item : mFontSizes) {
                if (ImGui::Selectable(toString(item).c_str(), counter == mSizeSelect)) {
                    mSizeSelect = counter;
                    CI_LOG_I("Read from Font:");
                    CI_LOG_I("SdfText::Font Height: " << mFonts[mFontSelect].mFonts[mSizeSelect].getHeight());
                    CI_LOG_I("SdfText::Font Leading: " << mFonts[mFontSelect].mFonts[mSizeSelect].getLeading());
                    CI_LOG_I("SdfText::Font Ascent: " << mFonts[mFontSelect].mFonts[mSizeSelect].getAscent());
                    CI_LOG_I("SdfText::Font Descent: " << mFonts[mFontSelect].mFonts[mSizeSelect].getDescent());
                    CI_LOG_I("SdfText::Font Size: " << mFonts[mFontSelect].mFonts[mSizeSelect].getSize());
                    CI_LOG_I("SdfText::Font Scale: " << mFonts[mFontSelect].mFonts[mSizeSelect].getFontScale());
                    CI_LOG_I("Read from SdfText:");
                    CI_LOG_I("SdfTextRef Height: " << mFonts[mFontSelect].mSdfTexts[mSizeSelect]->getHeight());
                    CI_LOG_I("SdfTextRef Leading: " << mFonts[mFontSelect].mSdfTexts[mSizeSelect]->getLeading());
                    CI_LOG_I("SdfTextRef Ascent: " << mFonts[mFontSelect].mSdfTexts[mSizeSelect]->getAscent());
                    CI_LOG_I("SdfTextRef Descent: " << mFonts[mFontSelect].mSdfTexts[mSizeSelect]->getDescent());
                    CI_LOG_I("SdfTextRef Size: " << mFonts[mFontSelect].mSdfTexts[mSizeSelect]->getSize());
                    CI_LOG_I("SdfTextRef Scale: " << mFonts[mFontSelect].mSdfTexts[mSizeSelect]->getFontScale());
                    CI_LOG_I("Read from SdfText->mFont");
                    CI_LOG_I("SdfText->getFont() Height: " << mFonts[mFontSelect].mSdfTexts[mSizeSelect]->getFont().getHeight());
                    CI_LOG_I("SdfText->getFont() Leading: " << mFonts[mFontSelect].mSdfTexts[mSizeSelect]->getFont().getLeading());
                    CI_LOG_I("SdfText->getFont() Ascent: " << mFonts[mFontSelect].mSdfTexts[mSizeSelect]->getFont().getAscent());
                    CI_LOG_I("SdfText->getFont() Descent: " << mFonts[mFontSelect].mSdfTexts[mSizeSelect]->getFont().getDescent());
                    CI_LOG_I("SdfText->getFont() Size: " << mFonts[mFontSelect].mSdfTexts[mSizeSelect]->getFont().getSize());
                    CI_LOG_I("SdfText->getFont() Scale: " << mFonts[mFontSelect].mSdfTexts[mSizeSelect]->getFont().getFontScale());
                }
                counter++;
            }
            ImGui::ListBoxFooter();
        }
    }
    if (ImGui::CollapsingHeader("Draw Options", ImGuiTreeNodeFlags_DefaultOpen)) {
        if (ImGui::ListBoxHeader("Anchor Alignment")) {
            int counter = 0;
            for (auto& item : mAlignmentLabels) {
                if (ImGui::Selectable(item.c_str(), counter == mAlignmentSelect)) {
                    mAlignmentSelect = counter;
                }
                counter++;
            }
            ImGui::ListBoxFooter();
        }
        if (ImGui::ListBoxHeader("Vertical Alignment")) {
            int counter = 0;
            for (auto& item : mVerticalAlignmentLabels) {
                if (ImGui::Selectable(item.c_str(), counter == mVerticalAlignmentSelect)) {
                    mVerticalAlignmentSelect = counter;
                }
                counter++;
            }
            ImGui::ListBoxFooter();
        }
        ImGui::SliderFloat("Leading (%)", &mLeading, 0.0f, 5.0f);
        ImGui::SliderFloat("Tracking (1/1000 em)", &mTracking, -1000.0f, 1000.0f);
        ImGui::Checkbox("Justify Text", &mJustify);
        ImGui::Checkbox("Premultiply Text", &mPremultiply);
        ImGui::Checkbox("Use Ligatures", &mLigate);
        ImGui::Checkbox("Use Half-Leading", &mUseHalfLeading);
    }
    if (ImGui::CollapsingHeader("Display Options", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Checkbox("Draw Bounding Boxes", &mDrawBoundingBoxes);
        ImGui::Checkbox("Draw Fit Rects", &mDrawFitRects);
        if (ImGui::Checkbox("Draw Ascender Line", &mDrawAscenderLine)) {
            CI_LOG_I("Ascent: " << mFonts[mFontSelect].mFonts[mSizeSelect].getAscent());        
        }
        if (ImGui::Checkbox("Draw Descender Line", &mDrawDescenderLine)) {
            CI_LOG_I("Descent: " << mFonts[mFontSelect].mFonts[mSizeSelect].getDescent());
        }
    }
    ImGui::End();
}

void TypographyAnatomyApp::draw() {
    float maxWidth = 0;
    gl::setMatricesWindow(getWindowSize());
    gl::enableAlphaBlending();
    gl::clear(Color(0, 0, 0));

    gl::SdfTextRef currentFont = mFonts[mFontSelect].mSdfTexts[mSizeSelect];
    ci::vec2 baseline(500, currentFont->getFont().getSize());

    gl::SdfText::DrawOptions options;

    options.tracking(mTracking);
    options.leading(mLeading);

    if (mJustify) {
        options.justify(true);
    }
    if (mPremultiply) {
        options.premultiply(true);
    }
    if (mLigate) {
        options.ligate(true);
    }
    if (mUseHalfLeading) {
        options.leadingStyle(gl::SdfText::HALF);
    }

    switch (mAlignmentSelect) {
        case 0:
            options.alignment(gl::SdfText::LEFT);
            break;
        case 1:
            options.alignment(gl::SdfText::CENTER);
            break;
        case 2:
            options.alignment(gl::SdfText::RIGHT);
            break;
    }

    switch (mVerticalAlignmentSelect) {
        case 0:
            options.verticalAlignment(gl::SdfText::TOP);
            break;
        case 1:
            options.verticalAlignment(gl::SdfText::MIDDLE);
            break;
        case 2:
            options.verticalAlignment(gl::SdfText::BOTTOM);
            break;
    }

    float lineHeight = currentFont->measureLineHeight(options);


    /*
    * Draw single lines using the simplest method
    * "baseline" refers to the bottom of the text for this method
    */

    for (auto& string : mSampleText) {
        gl::color(0.45f, 0.45f, 0.45f);
        gl::drawLine(vec2(0, baseline.y), vec2(getWindowWidth(), baseline.y));

        ci::Rectf boundingBox = currentFont->measureStringBounds(string, options);

        if (boundingBox.getWidth() > maxWidth) {
            maxWidth = boundingBox.getWidth();
        }

        gl::color(1, 1, 1);
        currentFont->drawString(string, baseline, options);

        if (mDrawBoundingBoxes) {
            ci::gl::pushMatrices();
            ci::gl::translate(baseline);
            gl::color(0, 0.45f, 0.45f);
            gl::drawStrokedRect(boundingBox);
            ci::gl::popMatrices();
        }


        if (mDrawAscenderLine) {
            gl::color(0.45f, 0, 0);
            gl::drawLine(vec2(0, baseline.y - currentFont->getAscent()),
                         vec2(getWindowWidth(), baseline.y - currentFont->getAscent()));
        }
        if (mDrawDescenderLine) {
            gl::color(0, 0.45f, 0);
            gl::drawLine(vec2(0, baseline.y + currentFont->getDescent()),
                         vec2(getWindowWidth(), baseline.y + currentFont->getDescent()));
        }


        baseline.y += lineHeight;    
    }

    /*
    * Single line with fitRect
    * "baseline" refers to the top edge of the fit rectangle here
    */
    gl::color(0.45f, 0.45f, 0.45f);
    gl::drawLine(vec2(0, baseline.y),
                 vec2(getWindowWidth(), baseline.y));

    ci::Rectf fitRect = ci::Rectf(
        ci::vec2(baseline),
        ci::vec2(baseline.x + 1.5*maxWidth, baseline.y + 2 * mLeading * currentFont->getFont().getSize()));

    if (mDrawBoundingBoxes) {
        gl::color(0, 0.45f, 0.45f);
        ci::gl::pushMatrices();
        ci::gl::translate(baseline);
        gl::drawStrokedRect(currentFont->measureStringBounds(mSampleText.front(), fitRect, options));
        ci::gl::popMatrices();
    }

    gl::color(1, 1, 1);
    ci::Rectf updatedRect = currentFont->drawString(mSampleText.front(), fitRect, ci::vec2(0), options);

    if (mDrawFitRects) {
        gl::color(0.45f, 0.45f, 0);
        gl::drawStrokedRect(fitRect);
        gl::color(0.45, 0, 0);
        gl::lineWidth(3.0f);
        gl::drawStrokedRect(updatedRect);
        gl::lineWidth(1.0f);
    }

    // skip two lines
    for (int i = 0; i < 2; i++) {
        baseline.y += lineHeight;
        //gl::color(0.45f, 0.45f, 0.45f);
        //gl::drawLine(vec2(0, baseline.y), vec2(getWindowWidth(), baseline.y));
    }

    /*
    * Paragraph of text with fitRect
    */
    ci::Rectf textBox(baseline, baseline + ci::vec2(maxWidth, 550));

    gl::color(0.45f, 0.45f, 0.45f);
    gl::drawLine(vec2(0, baseline.y), vec2(getWindowWidth(), baseline.y));

    if (mDrawBoundingBoxes) {
        gl::color(0, 0.45f, 0.45f);
        ci::gl::pushMatrices();
        ci::gl::translate(baseline);
        gl::drawStrokedRect(currentFont->measureStringBounds(mSampleParagraph, textBox, options));
        ci::gl::popMatrices();
    }

    gl::color(1, 1, 1);
    updatedRect = currentFont->drawString(mSampleParagraph, textBox, ci::vec2(0), options);

    if (mDrawFitRects) {
        gl::color(0.45f, 0.45f, 0);
        gl::drawStrokedRect(textBox);
        gl::color(0.45, 0, 0);
        gl::lineWidth(3.0f);
        gl::drawStrokedRect(updatedRect);
        gl::lineWidth(1.0f);
    }

    float leadingInPx = lineHeight - currentFont->getHeight();
    float offset = 0.5f * leadingInPx;
    baseline.y += lineHeight - offset;
    ci::gl::drawSolidCircle(baseline, 5);

    while (baseline.y <= updatedRect.y2) { 
        gl::color(0.45f, 0.45f, 0.45f);
        gl::drawLine(vec2(0, baseline.y),
                     vec2(getWindowWidth(), baseline.y));

        if (mDrawAscenderLine) {
            gl::color(0.45f, 0, 0);
            gl::drawLine(vec2(0, baseline.y - currentFont->getAscent()),
                            vec2(getWindowWidth(), baseline.y - currentFont->getAscent()));
        }
        if (mDrawDescenderLine) {
            gl::color(0, 0.45f, 0);
            gl::drawLine(vec2(0, baseline.y + currentFont->getDescent()),
                         vec2(getWindowWidth(), baseline.y + currentFont->getDescent()));
        }
        baseline.y += lineHeight;
    }

    gl::color(0.45f, 0.45f, 0.45f);
    gl::drawLine(vec2(0, baseline.y), vec2(getWindowWidth(), baseline.y));

    if (mDrawAscenderLine) {
        gl::color(0.45f, 0, 0);
        gl::drawLine(vec2(0, baseline.y - currentFont->getAscent()),
                     vec2(getWindowWidth(), baseline.y - currentFont->getAscent()));
    }
    if (mDrawDescenderLine) {
        gl::color(0, 0.45f, 0);
        gl::drawLine(vec2(0, baseline.y + currentFont->getDescent()),
                     vec2(getWindowWidth(), baseline.y + currentFont->getDescent()));
    }
}

CINDER_APP(TypographyAnatomyApp, RendererGl, [=](cinder::app::App::Settings* settings) {
    settings->setConsoleWindowEnabled();
    settings->setWindowSize(1920, 1080);
})
