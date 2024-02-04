#ifndef BEATGRID_HPP_INCLUDED
#define BEATGRID_HPP_INCLUDED

#include "Window.hpp"
#include "Widget.hpp"
#include "NanoVG.hpp"
#include <iostream>


START_NAMESPACE_DISTRHO


class BeatGrid : public NanoSubWidget
{
public:
    explicit BeatGrid(Widget *widget) noexcept;

protected:
    void onNanoDisplay() override;
    bool onMouse(const MouseEvent &) override;
    bool onMotion(const MotionEvent &) override;
    bool onScroll(const ScrollEvent &) override;

private:
    uint8_t (*pattern)[3][16];
    uint8_t sixteenth;

    DISTRHO_LEAK_DETECTOR(BeatGrid);

    friend class AITestUI;
};


END_NAMESPACE_DISTRHO
#endif