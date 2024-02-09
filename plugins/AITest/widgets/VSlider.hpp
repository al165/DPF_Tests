#ifndef VSLIDER_HPP_INCLUDED
#define VSLIDER_HPP_INCLUDED

#include "Window.hpp"
#include "Widget.hpp"
#include "NanoVG.hpp"
#include <iostream>

START_NAMESPACE_DISTRHO

class VSlider : public NanoSubWidget
{
public:
    class Callback {
        public:
            virtual ~Callback() {};
            virtual void vSliderDragStarted(VSlider *vSlider) = 0;
            virtual void vSliderDragFinished(VSlider *vSlider, float value) = 0;
            virtual void vSliderValueChanged(VSlider *vSlider, float value) = 0;
    };

    explicit VSlider(Widget *widget) noexcept;

    void setCallback(Callback *cb);
    void setValue(float val, bool sendCallback=false) noexcept;
    float getValue() noexcept;

    float min, max;
    float gauge_width;
    Color foreground_color, background_color, marker_color;

protected:
    void onNanoDisplay() override;
    bool onMouse(const MouseEvent &) override;
    bool onMotion(const MotionEvent &) override;
    bool onScroll(const ScrollEvent &) override;

private:
    Callback *callback;
    bool dragging_;
    float value_, tmp_value_;
    int mouseY_;
    int last_mouse_y_;

    DISTRHO_LEAK_DETECTOR(VSlider);
};



END_NAMESPACE_DISTRHO

#endif