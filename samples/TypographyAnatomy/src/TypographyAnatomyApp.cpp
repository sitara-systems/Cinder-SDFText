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
    int mFontSelect;
    int mSizeSelect;

    bool mDrawBoundingBoxes;

    bool mPremultiply = false;
    gl::SdfText::Alignment mAlignment = gl::SdfText::Alignment::LEFT;
    bool mJustify = false;
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
    mSampleText.push_back("The five boxing wizards jump quickly.");
    mSampleText.push_back("Pack my box with five dozen liquor jugs.");
    mSampleText.push_back("Fix problem quickly with galvanized jets.");

    mFontSelect = 0;
    mSizeSelect = 0;
    mDrawBoundingBoxes = false;

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
                }
                counter++;
            }
            ImGui::ListBoxFooter();
        }
    }
    if (ImGui::CollapsingHeader("Draw Options", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Checkbox("Justify Text", &mJustify);
        ImGui::Checkbox("Premultiply Text", &mPremultiply);
    }
    if (ImGui::CollapsingHeader("Display Options", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Checkbox("Draw Bounding Boxes", &mDrawBoundingBoxes);
    }
    ImGui::End();
}

void TypographyAnatomyApp::draw() {
    gl::setMatricesWindow(getWindowSize());
    gl::enableAlphaBlending();
    gl::clear(Color(0, 0, 0));

    gl::SdfTextRef currentFont = mFonts[mFontSelect].mSdfTexts[mSizeSelect];
    ci::vec2 baseline(500, 500);

    for (auto& string : mSampleText) {
        gl::color(0.45f, 0.45f, 0.45f);
        gl::drawLine(vec2(0, baseline.y), vec2(getWindowWidth(), baseline.y));

        gl::color(0, 1, 0);
        if (mDrawBoundingBoxes) {
            ci::gl::pushMatrices();
            ci::gl::translate(baseline);
            gl::drawStrokedRect(currentFont->measureStringBounds(string));
            ci::gl::popMatrices();
        }

        gl::color(1, 1, 1);
        currentFont->drawString(string, baseline);

        baseline.y += currentFont->getHeight();    
    }    
}

CINDER_APP(TypographyAnatomyApp, RendererGl, [=](cinder::app::App::Settings* settings) {
    settings->setConsoleWindowEnabled();
    settings->setWindowSize(1920, 1080);
})
