#ifndef SIMPLEBUTTON_HPP_INCLUDED
#define SIMPLEBUTTON_HPP_INCLUDED

#include "NanoVG.hpp"
#include "EventHandlers.hpp"

#include <string>

START_NAMESPACE_DGL

class Button : public NanoSubWidget,
               public ButtonEventHandler
{
public:
    explicit Button(Widget* parent, ButtonEventHandler::Callback *cb);
    ~Button() override;

    void setBackgroundColor(Color color);
    void setFontScale(float scale);
    void setLabel(const std::string& label);
    void setLabelColor(Color color);

protected:
    void onNanoDisplay() override;
    bool onMouse(const MouseEvent& ev) override;
    bool onMotion(const MotionEvent& ev) override;

private:
    Color backgroundColor;
    Color labelColor;
    std::string label;
    float fontScale;

    DISTRHO_LEAK_DETECTOR(Button)

};

END_NAMESPACE_DGL

#endif