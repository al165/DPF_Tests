#include "BeatGrid.hpp"

START_NAMESPACE_DISTRHO

using DGL_NAMESPACE::Color;

BeatGrid::BeatGrid(Widget *parent) noexcept
    : NanoSubWidget(parent)
{
    loadSharedResources();

    sixteenth = 0;
}

bool BeatGrid::onMouse(const MouseEvent &ev){ return false; }
bool BeatGrid::onMotion(const MotionEvent &ev){ return false; }
bool BeatGrid::onScroll(const ScrollEvent &ev){ return false; }



void BeatGrid::onNanoDisplay()
{
    const float width = getWidth();
    const float height = getHeight();

    const float cw = width/16.0f;
    const float ch = height/3.0f;
    
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 16; j++)
        {
            uint8_t c = 2*(127 - (*pattern)[i][j]);
            Color highlight = Color(10, 10, 10);
            if(j == sixteenth){
                highlight = Color(240, 107, 29);
            }

            beginPath();
            strokeColor(highlight);
            fillColor(Color(c, c, c));
            rect(j*cw, i*ch, cw-1, ch-1);
            fill();
            stroke();
            closePath();
        }
    }
}


END_NAMESPACE_DISTRHO