#include "SimpleButton.hpp"
#include "Window.hpp"

START_NAMESPACE_DGL

Button::Button(Widget* const parent, ButtonEventHandler::Callback* const cb)
    : NanoSubWidget(parent),
      ButtonEventHandler(this),
      backgroundColor(32, 32, 32),
      labelColor(255, 255, 255),
      label("button"),
      fontScale(1.0f)
{
#ifdef DGL_NO_SHARED_RESOURCES
    createFontFromFile("sans", "/usr/share/fonts/truetype/ttf-dejavu/DejaVuSans.ttf");
#else
    loadSharedResources();
#endif
    ButtonEventHandler::setCallback(cb);
}

Button::~Button()
{
}

void Button::setBackgroundColor(const Color color)
{
    backgroundColor = color;
}

void Button::setFontScale(const float scale)
{
    fontScale = scale;
}

void Button::setLabel(const std::string& label2)
{
    label = label2;
}

void Button::setLabelColor(const Color color)
{
    labelColor = color;
}

void Button::onNanoDisplay()
{
    const uint w = getWidth();
    const uint h = getHeight();
    const float margin = 1.0f;

    // Background
    beginPath();
    fillColor(backgroundColor);
    strokeColor(labelColor);
    rect(margin, margin, w - 2 * margin, h - 2 * margin);
    fill();
    stroke();
    closePath();

    // Label
    beginPath();
    fontSize(14 * fontScale);
    fillColor(labelColor);
    Rectangle<float> bounds;
    textBounds(0, 0, label.c_str(), NULL, bounds);
    float tx = w / 2.0f ;
    float ty = h / 2.0f;
    textAlign(ALIGN_CENTER | ALIGN_MIDDLE);

    fillColor(255, 255, 255, 255);
    text(tx, ty, label.c_str(), NULL);
    closePath();
}

bool Button::onMouse(const MouseEvent& ev)
{
    return ButtonEventHandler::mouseEvent(ev);
}

bool Button::onMotion(const MotionEvent& ev)
{
    return ButtonEventHandler::motionEvent(ev);
}


END_NAMESPACE_DGL