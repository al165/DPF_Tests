#include "SamplePlayerUI.hpp"
#include "SimpleButton.hpp"


START_NAMESPACE_DISTRHO

using DGL_NAMESPACE::Color;
using DGL_NAMESPACE::Button;
using DGL_NAMESPACE::ButtonEventHandler;
using DGL_NAMESPACE::SubWidget;

SamplePlayerUI::SamplePlayerUI()
    : UI(UI_W, UI_H),
      fButton(this, this)
{
    plugin = static_cast<SamplePlayer *>(getPluginInstancePointer());
    fButton.setAbsolutePos(10, 10);
    fButton.setLabel("Open...");
    fButton.setSize(100, 30);

    loadSharedResources();
}

SamplePlayerUI::~SamplePlayerUI()
{

}

void SamplePlayerUI::parameterChanged(uint32_t index, float value)
{
    switch(index)
    {
        case kAmp:
            break;
        case kMidiNote:
            break;
        case kSampleLoaded:
            break;
        default:
            break;
    }

    printf("parameterChanged: %d %f\n", index, value);
}

void SamplePlayerUI::onNanoDisplay()
{
    // printf("onNanoDisplay: fileName = %s\n", fileName);
    float width = getWidth();
    float height = getHeight();

    fontSize(15.0f);

    beginPath();
    fillColor(Color(37, 36, 34));
    rect(0.0f, 0.0f, width, height);
    fill();
    closePath();

    if(fileName.length() > 0){
        beginPath();
        fillColor(200, 200, 200);
        textAlign(ALIGN_LEFT|ALIGN_TOP);
        textBox(10, 100, UI_W - 10, fileName);
        closePath();
    }
}

void SamplePlayerUI::stateChanged(const char *key, const char *value)
{
    printf("stateChanged: %s -> %s\n", key, value);

    if (strcmp(key, "filepath") == 0)
    {
        fileName = String(value);
        printf("  filename: %s\n", fileName);
    }

    repaint();
}


void SamplePlayerUI::buttonClicked(SubWidget* const widget, int) 
{
    printf("buttonClicked\n");
    requestStateFile("filepath");
}


UI *createUI()
{
    return new SamplePlayerUI();
}

END_NAMESPACE_DISTRHO