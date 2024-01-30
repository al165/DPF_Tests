#include "SamplePlayerUI.hpp"


START_NAMESPACE_DISTRHO

using DGL_NAMESPACE::Color;


SamplePlayerUI::SamplePlayerUI()
    : UI(UI_W, UI_H)
{
    plugin = static_cast<SamplePlayer *>(getPluginInstancePointer());

    Window &window = getParentWindow();

    fButton = new Button(this);
    fButton->setAbsolutePos(10, 10);
    fButton->setLabel("Open...");
    fButton->setSize(100, 30);
    fButton->setCallback(this);


    const uint sz = 60;
    const Size<uint> knobSize = Size<uint>(sz, sz);

    fAmp = new Knob(this);
    fAmp->setId(kAmp);
    fAmp->setSize(knobSize);
    fAmp->setCallback(this);
    fAmp->gauge_width = 12.0f;
    fAmp->setAbsolutePos(130, 30);
    fAmp->max = 2.0f;

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
            fAmp->setValue(value);
            repaint();
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


void SamplePlayerUI::buttonClicked(Button *button)
{
    printf("buttonClicked\n");
    requestStateFile("filepath");
}

void SamplePlayerUI::knobDragStarted(Knob *knob)
{

}

void SamplePlayerUI::knobDragFinished(Knob *knob, float value)
{
    uint id = knob->getId();

    switch(id)
    {
        case kAmp:
            setParameterValue(kAmp, value);
            break;
        default:
            break;
    }
    repaint();
}

void SamplePlayerUI::knobValueChanged(Knob *knob, float value)
{
    uint id = knob->getId();

    switch(id)
    {
        case kAmp:
            setParameterValue(kAmp, value);
            break;
        default:
            break;        
    }
    repaint();
}


UI *createUI()
{
    return new SamplePlayerUI();
}

END_NAMESPACE_DISTRHO