#ifndef AITEST_UI_HPP_INCLUDED
#define AITEST_UI_HPP_INCLUDED 

#include "DistrhoUI.hpp"
#include "NanoVG.hpp"
#include "Window.hpp"

#include "AITest.hpp"
#include "SimpleButton.hpp"
#include "Knob.hpp"
#include "HBox.hpp"
#include "BeatGrid.hpp"


START_NAMESPACE_DISTRHO

constexpr unsigned int UI_W = 600;
constexpr unsigned int UI_H = 200;

class AITestUI : public UI,
                 public Button::Callback,
                 public Knob::Callback
{
public:
    AITestUI();
    ~AITestUI();

protected:
    void parameterChanged(uint32_t index, float value) override;
    void stateChanged(const char *key, const char *value) override;
    void onNanoDisplay() override;
    void buttonClicked(Button *button) override;
    void knobDragStarted(Knob *knob) override;
    void knobDragFinished(Knob *knob, float value) override;
    void knobValueChanged(Knob *knob, float value) override;


private:
    AITest *plugin;
    HBox *hbox_controls;
    Button *fGenerate;
    Knob *fThreshold;
    BeatGrid *fBeatGrid;

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AITestUI);
};

END_NAMESPACE_DISTRHO
#endif