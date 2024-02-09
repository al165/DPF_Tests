#include "XYSlider.hpp"

START_NAMESPACE_DISTRHO

using DGL_NAMESPACE::Color;


XYSlider::XYSlider(Widget *parent) noexcept
    : NanoSubWidget(parent)
{
    loadSharedResources();

    dragging_ = false;
    value_x_ = 0.0f;
    value_y_ = 0.0f;
    minX = minY = 0.0f;
    maxX = maxY = 1.0f;
    fg_color = Color(40, 40, 40);
    bg_color = Color(255, 255, 255);
}

float XYSlider::getXValue() noexcept
{
    return value_x_;
}

float XYSlider::getYValue() noexcept
{
    return value_y_;
}

void XYSlider::setXValue(float x, bool sendCallback) noexcept
{
    if(x == value_x_) return;

    value_x_ = x;

    if(sendCallback && callback != nullptr)
    {
        callback->xyValueChanged(this, value_x_, value_y_);
    }
}

void XYSlider::setYValue(float y, bool sendCallback) noexcept
{
    if(y == value_y_) return;

    value_y_ = y;

    if(sendCallback && callback != nullptr)
    {
        callback->xyValueChanged(this, value_x_, value_y_);
    }
}

bool XYSlider::onMouse(const MouseEvent &ev)
{
    if(!isVisible() || ev.button != 1) return false;

    if(ev.press)
    {
        if(!contains(ev.pos)) return false;

        dragging_ = true;

        if(callback != nullptr)
        {
            callback->xyDragStarted(this);
        }

        updateXY(ev.pos);
        return false;
    }
    else if(dragging_)
    {
        dragging_ = false;
    }

    return false;
}

bool XYSlider::onMotion(const MotionEvent &ev)
{
    if(!isVisible()) return false;

    if(!dragging_) return false;

    // printf("pos.x: %.1f pos.y: %.1f\n", ev.pos.getX(), ev.pos.getY());
    updateXY(ev.pos);
    

    return false;
}

void XYSlider::updateXY(const DGL::Point<double> pos)
{
    const float width = getWidth();
    const float height = getHeight();

    float x_ = std::clamp(pos.getX() / (double)width, 0.0, 1.0);
    float y_ = std::clamp(pos.getY() / (double)height, 0.0, 1.0);

    float value_x = (maxX - minX) * x_ + minX;
    float value_y = (maxY - minY) * y_ + minY;

    // printf("x_: %.1f, y_: %.1f, value_x: %.1f, value_y: %.1f\n", x_, y_, value_x, value_y);

    setXValue(value_x, false);
    setYValue(value_y, true);
}

bool XYSlider::onScroll(const ScrollEvent &ev)
{
    return false;
}

void XYSlider::onNanoDisplay()
{
    const float width = getWidth();
    const float height = getHeight();

    beginPath();
    fillColor(bg_color);
    strokeColor(fg_color);
    rect(0, 0, width, height);
    fill();
    stroke();
    closePath();

    float x_ = (value_x_ - minX) / (maxX - minX);
    float y_ = (value_y_ - minY) / (maxY - minY);

    beginPath();
    fillColor(marker_color);
    // rect(x_*width - 4.0f, y_*height - 4.0f, 8.0f, 8.0f);
    circle(x_*width, y_*height, 5);
    fill();
    closePath();

    beginPath();
    strokeColor(fg_color);
    moveTo(0, y_*height);
    lineTo(width, y_*height);
    // rect(0, y_*height, width, 1);
    stroke();
    closePath();

    beginPath();
    strokeColor(fg_color);
    moveTo(x_*width, 0);
    lineTo(x_*width, height);
    stroke();
    closePath();
}

void XYSlider::setCallback(Callback *cb)
{
    callback = cb;
}


END_NAMESPACE_DISTRHO