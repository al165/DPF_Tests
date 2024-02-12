#ifndef AITEST_UI_HPP_INCLUDED
#define AITEST_UI_HPP_INCLUDED 

#include "DistrhoUI.hpp"
#include "NanoVG.hpp"
#include "Window.hpp"

#include "AITest.hpp"
#include "SimpleButton.hpp"
#include "Slider.hpp"
#include "VSlider.hpp"
#include "HBox.hpp"
#include "BeatGrid.hpp"
#include "XYSlider.hpp"

#include "font.cpp"


START_NAMESPACE_DISTRHO

constexpr unsigned int UI_W = 800;
constexpr unsigned int UI_H = 280;

class AITestUI : public UI,
                 public Button::Callback,
                 public Slider::Callback,
                 public XYSlider::Callback
{
public:
    AITestUI();
    ~AITestUI();

protected:
    void parameterChanged(uint32_t index, float value) override;
    void stateChanged(const char *key, const char *value) override;
    void onNanoDisplay() override;
    void buttonClicked(Button *button) override;
    void sliderDragStarted(Slider *slider) override;
    void sliderDragFinished(Slider *slider, float value) override;
    void sliderValueChanged(Slider *slider, float value) override;
    void xyDragStarted(XYSlider *xySlider) override;
    void xyDragFinished(XYSlider *xySlider, float x, float y) override;
    void xyValueChanged(XYSlider *xySlider, float x, float y) override;


private:
    AITest *plugin;
    HBox *hbox_controls;
    Button *fGenerate;
    VSlider *fThreshold;
    BeatGrid *fBeatGrid;
    XYSlider *fXYSlider;

    FontId _logo_font;

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AITestUI);
};

END_NAMESPACE_DISTRHO
#endif