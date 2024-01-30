#include "SamplePlayerUI.hpp"


START_NAMESPACE_DISTRHO

using DGL_NAMESPACE::Color;


SamplePlayerUI::SamplePlayerUI()
    : UI(UI_W, UI_H)
{
    plugin = static_cast<SamplePlayer *>(getPluginInstancePointer());

    Window &window = getParentWindow();

    hbox_controls = new HBox(this);

    fButton = new Button(hbox_controls);
    fButton->setLabel("Open...");
    fButton->setSize(100, 30);
    fButton->setCallback(this);


    const uint sz = 60;
    const Size<uint> knobSize = Size<uint>(sz, sz);

    fAmp = new Knob(hbox_controls);
    fAmp->setId(kAmp);
    fAmp->setSize(knobSize);
    fAmp->setCallback(this);
    fAmp->gauge_width = 12.0f;
    fAmp->max = 2.0f;

    fMidiNumber = new Knob(hbox_controls);
    fMidiNumber->setId(kMidiNote);
    fMidiNumber->setSize(knobSize);
    fMidiNumber->setCallback(this);
    fMidiNumber->gauge_width = 12.0f;
    fMidiNumber->max = 128.0f;

    hbox_controls->setAbsolutePos(0, 0);
    hbox_controls->setWidth(UI_W);
    hbox_controls->padding = 10;
    hbox_controls->justify_content = HBox::Justify_Content::left;
    hbox_controls->addWidget(fButton);
    hbox_controls->addWidget(fAmp);
    hbox_controls->addWidget(fMidiNumber);
    hbox_controls->positionWidgets();


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
            fMidiNumber->setValue(value);
            repaint();
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