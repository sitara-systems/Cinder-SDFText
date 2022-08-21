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
    float mTracking;
    float mLeading;

    bool mDrawBoundingBoxes;
    bool mDrawAscenderLine;
    bool mDrawDescenderLine;

    std::vector<std::string> mAlignmentLabels = {"Left", "Center", "Right"};
    bool mJustify = false;
    bool mPremultiply = false;
    bool mLigate = false;
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
    mSampleText.push_back("A very bad quack might jinx zippy fowls.");
    mSampleText.push_back("The five boxing wizards jumped quickly.");
    mSampleText.push_back("Pack my box with five dozen liquor jugs.");
    mSampleText.push_back("Fix problems quickly with galvanized jets.");
    mSampleText.push_back("Ligatures: fb ff fi fj fk fl ft ffb ffi ffj ffk ffl fft Th");

    mSampleParagraph =
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nullam in velit vehicula, porta ligula ut, luctus "
        "justo. Aliquam vitae ullamcorper odio. Nullam placerat erat nunc, id faucibus nisl pretium eget. Integer a "
        "nisi laoreet, dignissim leo facilisis, iaculis est. Aliquam eu fermentum felis. Morbi pulvinar magna et "
        "pulvinar ultricies. Cras turpis est, rhoncus mollis luctus in, tristique ac quam. Cras molestie urna augue, "
        "at molestie urna fringilla nec.";

    mFontSelect = 0;
    mSizeSelect = 0;
    mAlignmentSelect = 0;
    mDrawBoundingBoxes = false;
    mDrawAscenderLine = false;
    mDrawDescenderLine = false;
    mTracking = 0.0f;
    mLeading = 38.0f;

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
                    CI_LOG_I("Font Height: " << mFonts[mFontSelect].mFonts[mSizeSelect].getHeight());
                    CI_LOG_I("Font Leading: " << mFonts[mFontSelect].mFonts[mSizeSelect].getLeading());
                    CI_LOG_I("Font Ascent: " << mFonts[mFontSelect].mFonts[mSizeSelect].getAscent());
                    CI_LOG_I("Font Descent: " << mFonts[mFontSelect].mFonts[mSizeSelect].getDescent());
                    CI_LOG_I("Font Size: " << mFonts[mFontSelect].mFonts[mSizeSelect].getSize());
                    CI_LOG_I("Font Scale: " << mFonts[mFontSelect].mFonts[mSizeSelect].getFontScale());
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
        ImGui::SliderFloat("Leading", &mLeading, 0.0f, 1000.0f);
        ImGui::SliderFloat("Tracking", &mTracking, 0.0f, 1000.0f);
        ImGui::Checkbox("Justify Text", &mJustify);
        ImGui::Checkbox("Premultiply Text", &mPremultiply);
        ImGui::Checkbox("Use Ligatures", &mLigate);
    }
    if (ImGui::CollapsingHeader("Display Options", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Checkbox("Draw Bounding Boxes", &mDrawBoundingBoxes);
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
    ci::vec2 baseline(500, currentFont->getHeight());

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

    for (auto& string : mSampleText) {
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

        gl::color(0, 0, 1.0f);
        ci::Rectf textBox = currentFont->measureStringBounds(string, options);
        if (textBox.getWidth() > maxWidth) {
            maxWidth = textBox.getWidth();
        }
        if (mDrawBoundingBoxes) {
            ci::gl::pushMatrices();
            ci::gl::translate(baseline);
            gl::drawStrokedRect(textBox);
            ci::gl::popMatrices();
        }

        gl::color(1, 1, 1);
        currentFont->drawString(string, baseline, options);

        baseline.y += 2.0f*currentFont->getHeight();    
    }

    ci::Rectf textBox(baseline, baseline + ci::vec2(maxWidth, 750));

    gl::color(0.45f, 0.45f, 0.45f);
    gl::drawLine(vec2(0, baseline.y), vec2(getWindowWidth(), baseline.y));

    if (mDrawBoundingBoxes) {
        gl::color(0, 0, 1.0f);
        ci::gl::pushMatrices();
        ci::gl::translate(baseline);
        gl::drawStrokedRect(currentFont->measureStringBoundsWrapped(mSampleParagraph, textBox, options));
        ci::gl::popMatrices();
    }

    gl::color(1, 1, 1);
    currentFont->drawStringWrapped(mSampleParagraph, textBox, ci::vec2(0), options);
}

CINDER_APP(TypographyAnatomyApp, RendererGl, [=](cinder::app::App::Settings* settings) {
    settings->setConsoleWindowEnabled();
    settings->setWindowSize(1920, 1080);
})
