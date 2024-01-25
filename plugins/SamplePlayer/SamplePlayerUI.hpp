#ifndef SAMPLEPLAYER_UI_HPP_INCLUDED
#define SAMPLEPLAYER_UI_HPP_INCLUDED

#include "DistrhoUI.hpp"
#include "NanoVG.hpp"
#include "Window.hpp"

#include "SamplePlayer.hpp"
#include "SimpleButton.hpp"


START_NAMESPACE_DISTRHO

constexpr unsigned int UI_W = 1000;
constexpr unsigned int UI_H = 550;

using DGL_NAMESPACE::Button;
using DGL_NAMESPACE::ButtonEventHandler;
using DGL_NAMESPACE::SubWidget;

class SamplePlayerUI : public UI,
                       public ButtonEventHandler::Callback
{
public:
    SamplePlayerUI();
    ~SamplePlayerUI();

protected:
    void parameterChanged(uint32_t index, float value) override;
    void stateChanged(const char *key, const char *value) override;
    void onNanoDisplay() override;
    void buttonClicked(SubWidget* const widget, int) override;

private:
    SamplePlayer *plugin;
    String fileName;
    Button fButton;

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SamplePlayerUI);
};


END_NAMESPACE_DISTRHO
#endif