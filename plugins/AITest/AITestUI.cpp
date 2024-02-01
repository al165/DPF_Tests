#include "AITestUI.hpp"

START_NAMESPACE_DISTRHO

using DGL_NAMESPACE::Color;

AITestUI::AITestUI()
    : UI(UI_W, UI_H)
{
    // plugin = static_cast<AITest *>(getPluginInstancePointer());

    Window &window = getWindow();

    hbox_controls = new HBox(this);

    fGenerate = new Button(hbox_controls);
    fGenerate->setLabel("generate");
    fGenerate->setSize(100, 30);
    fGenerate->setAbsolutePos(20, 20);
    fGenerate->setCallback(this);
    fGenerate->setId(kGenerate);

    loadSharedResources();
}

AITestUI::~AITestUI()
{

}

void AITestUI::parameterChanged(uint32_t index, float value)
{
    switch(index)
    {
        default:
            break;
    }
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
}

// void AITestUI::stateChanged(const char *key, const char *value)
// {
//     repaint();
// }

void AITestUI::buttonClicked(Button *button)
{
    printf("buttonClicked!\n");
    setParameterValue(kGenerate, 1.0f);
}

UI *createUI()
{
    return new AITestUI();
}


END_NAMESPACE_DISTRHO