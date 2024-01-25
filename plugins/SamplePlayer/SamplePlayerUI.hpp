#ifndef SAMPLEPLAYER_UI_HPP_INCLUDED
#define SAMPLEPLAYER_UI_HPP_INCLUDED

#include "DistrhoUI.hpp"
#include "NanoVG.hpp"
#include "Window.hpp"

#include "SamplePlayer.hpp"


START_NAMESPACE_DISTRHO

constexpr unsigned int UI_W = 1000;
constexpr unsigned int UI_H = 550;

class SamplePlayerUI : public UI
{
public:
    SamplePlayerUI();
    ~SamplePlayerUI();

protected:
    void parameterChanged(uint32_t index, float value) override;
    void stateChanged(const char *key, const char *value) override;
    void onNanoDisplay() override;
    bool onMouse(const MouseEvent &ev) override;
    bool onScroll(const ScrollEvent &ev) override;
    bool onMotion(const MotionEvent &ev) override;

private:
    SamplePlayer *plugin;
    std::string fileName;
};


END_NAMESPACE_DISTRHO
#endif