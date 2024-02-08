#include "AITestUI.hpp"

START_NAMESPACE_DISTRHO

using DGL_NAMESPACE::Color;

AITestUI::AITestUI()
    : UI(UI_W, UI_H)
{
    plugin = static_cast<AITest *>(getPluginInstancePointer());

    Window &window = getWindow();

    hbox_controls = new HBox(this);

    fGenerate = new Button(hbox_controls);
    fGenerate->setLabel("generate");
    fGenerate->setSize(100, 30);
    fGenerate->setCallback(this);
    fGenerate->setId(kGenerate);

    fThreshold = new Knob(hbox_controls);
    fThreshold->setId(kThreshold);
    fThreshold->setSize(Size<uint>(50, 50));
    fThreshold->setCallback(this);
    fThreshold->gauge_width = 12.0f;
    fThreshold->max = 1.0f;

    hbox_controls->setAbsolutePos(10, 10);
    hbox_controls->setWidth(UI_W);
    hbox_controls->padding = 10;
    hbox_controls->justify_content = HBox::Justify_Content::left;
    hbox_controls->addWidget(fGenerate);
    hbox_controls->addWidget(fThreshold);
    hbox_controls->positionWidgets();

    fBeatGrid = new BeatGrid(this);
    fBeatGrid->setSize(Size<uint>(UI_W - 20, 80));
    fBeatGrid->setAbsolutePos(10, 80);

    loadSharedResources();

    fBeatGrid->pattern = &plugin->pattern;

    setGeometryConstraints(UI_W, UI_H, true, true);
}

AITestUI::~AITestUI()
{

}

void AITestUI::parameterChanged(uint32_t index, float value)
{
    // printf("AITestUI::parameterChanged: %d %.2f\n", index, value);
    switch(index)
    {   
        case kThreshold:
            fThreshold->setValue(value);
            repaint();
            break;
        case kSixteenth:
            fBeatGrid->sixteenth = (uint8_t)value;
            fBeatGrid->repaint();
            break;
        default:
            break;
    }
}

void AITestUI::stateChanged(const char *key, const char *value)
{
    printf("AITestUI::stateChanged()\n");

    repaint();
}

void AITestUI::onNanoDisplay()
{
    float width = getWidth();
    float height = getHeight();

    beginPath();
    fillColor(Color(240, 240, 240));
    rect(0.0f, 0.0f, width, height);
    fill();
    closePath();

    char build_date[80];
    sprintf(build_date, "Build: %s %s", __DATE__, __TIME__);

    beginPath();
    fillColor(Color(40, 40, 40));
    fontSize(12);
    textAlign(Align::ALIGN_RIGHT);
    text(width-10, height-10, build_date, NULL);
    closePath();
}



void AITestUI::buttonClicked(Button *button)
{
    setParameterValue(kGenerate, 1.0f);
    repaint();
}

void AITestUI::knobDragStarted(Knob *knob){}

void AITestUI::knobDragFinished(Knob *knob, float value)
{
    // uint id = knob->getId();
    // setParameterValue(id, value);
    // repaint();
}

void AITestUI::knobValueChanged(Knob *knob, float value)
{
    uint id = knob->getId();
    setParameterValue(id, value);
    repaint();
}

UI *createUI()
{
    return new AITestUI();
}


END_NAMESPACE_DISTRHO