#include "SamplePlayerUI.hpp"


START_NAMESPACE_DISTRHO

using DGL_NAMESPACE::Color;

SamplePlayerUI::SamplePlayerUI()
    : UI(UI_W, UI_H)
{
    plugin = static_cast<SamplePlayer *>(getPluginInstancePointer());


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

    printf("parameterChanged: %f", value);
}

void SamplePlayerUI::onNanoDisplay()
{
    float width = getWidth();
    float height = getHeight();
    // background
    beginPath();
    fillColor(Color(37, 36, 34));
    rect(0.0f, 0.0f, width, height);
    fill();
    closePath();
}

void SamplePlayerUI::stateChanged(const char *key, const char *value)
{
    if (strcmp(key, "filepath") == 0)
    {
        fileName = std::string(value);
    }
}

bool SamplePlayerUI::onMouse(const MouseEvent &ev)
{
    printf("onMouse: x = %f", ev.pos.getX());
    return false;
}

bool SamplePlayerUI::onScroll(const ScrollEvent &ev)
{
    printf("onScroll: x = %f", ev.pos.getX());
    return false;
}

bool SamplePlayerUI::onMotion(const MotionEvent &ev)
{
    printf("onMotion: x = %f", ev.pos.getX());
    return false;
}


UI *createUI()
{
    return new SamplePlayerUI();
}

END_NAMESPACE_DISTRHO