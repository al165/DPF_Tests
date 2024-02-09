#include "VSlider.hpp"

START_NAMESPACE_DISTRHO

using DGL_NAMESPACE::Color;


VSlider::VSlider(Widget *parent) noexcept
    : NanoSubWidget(parent)
{
    loadSharedResources();

    dragging_ = false;
    value_ = 0.0f;
    tmp_value_ = 0.0f;
    min = 0.0f;
    max = 1.0f;
    foreground_color = Color(255, 255, 255);
    background_color = Color(0, 0, 0);
    gauge_width = 6.0f;
}

float VSlider::getValue() noexcept
{
    return value_;
}

void VSlider::setValue(float val, bool sendCallback) noexcept
{
    if(val == value_) return;

    value_ = std::max(min, std::min(max, val));
    tmp_value_ = value_;

    if(sendCallback && callback != nullptr)
    {
        callback->vSliderValueChanged(this, value_);
    }
}

bool VSlider::onMouse(const MouseEvent &ev)
{
    if(!isVisible() || ev.button != 1) return false;

    if(ev.press)
    {
        if(!contains(ev.pos)) return false;

        dragging_ = true;

        if(callback != nullptr)
        {
            callback->vSliderDragStarted(this);
        }
        repaint();
        return false;
    } 
    else if(dragging_)
    {
        dragging_ = false;
    }

    return false;
}

bool VSlider::onMotion(const MotionEvent &ev)
{
    if(!isVisible()) return false;
    

    if(!dragging_) return false;

    float d, value = 0.0f;
    d = 200.f;
    const int movY = last_mouse_y_ - ev.pos.getY();
    value = tmp_value_ - (float(min - max)/d * float(movY));

    if(value < min) tmp_value_ = value = min;
    else if(value > max) tmp_value_ = value = max;

    setValue(value, true);
    last_mouse_y_ = ev.pos.getY();

    return false;
}

bool VSlider::onScroll(const ScrollEvent &ev)
{
    return false;
}

void VSlider::onNanoDisplay()
{
    const float width = getWidth();
    const float height = getHeight();

    float normValue = (value_ - min) / (max - min);
    if(normValue < 0.0f) normValue = 0.0f;

    
    beginPath();
    fillColor(background_color);
    strokeColor(foreground_color);
    rect(0, 0, width, height);
    fill();
    stroke();
    closePath();

    beginPath();
    strokeColor(foreground_color);
    moveTo(width/2.0f, height);
    lineTo(width/2.0f, (1.0f - normValue)*height);
    stroke();
    closePath();

    beginPath();
    fillColor(marker_color);
    circle(width/2.0f, (1.0f - normValue)*height, 5);
    fill();
    closePath();


}

void VSlider::setCallback(Callback *cb)
{
    callback = cb;
}

END_NAMESPACE_DISTRHO