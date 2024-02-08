#include "BeatGrid.hpp"

START_NAMESPACE_DISTRHO

using DGL_NAMESPACE::Color;

BeatGrid::BeatGrid(Widget *parent) noexcept
    : NanoSubWidget(parent)
{
    loadSharedResources();

    sixteenth = 0;

    cellBackground = Color(255, 255, 255);
    cellBorder = Color(10, 10, 10);
    triggerBackground = Color(200, 200, 200);
    triggerForeground = Color(128, 128, 128);
    triggerHighlightBackground = Color(243, 212, 63);
    triggerHighlightForeground = Color(128, 128, 128);
}

bool BeatGrid::onMouse(const MouseEvent &ev){ return false; }
bool BeatGrid::onMotion(const MotionEvent &ev){ return false; }
bool BeatGrid::onScroll(const ScrollEvent &ev){ return false; }


void BeatGrid::onNanoDisplay()
{
    const float width = getWidth();
    const float height = getHeight();

    const float cw = (width - 80.0f)/16.0f;
    const float ch = height/3.0f;
    
    for(int i = 0; i < 3; i++)
    {
        beginPath();
        fillColor(Color(40, 40, 40));
        fontFaceId(_font);
        textAlign(Align::ALIGN_RIGHT | Align::ALIGN_MIDDLE);
        text(70.0f, (2.5-i)*ch, channelNames[i], nullptr);
        closePath();

        for(int j = 0; j < 16; j++)
        {
            float v = (float)(*pattern)[i][j]/128.0f;
            Color *bg = &cellBackground;
            Color *fg = &triggerForeground;
            if(v > 0.0f)
            {
                bg = &triggerBackground;
            }
            if(j == sixteenth){
                bg = &triggerHighlightBackground;
                fg = &triggerHighlightForeground;
            }

            beginPath();
            strokeColor(cellBorder);
            fillColor(*bg);
            rect(80.0f + j*cw, (2-i)*ch, cw-1, ch-1);
            fill();
            stroke();
            closePath();

            if(v > 0.0f)
            {
                beginPath();
                fillColor(*fg);
                rect(80.0f + j*cw, (2.0f-i)*ch + (1.0f-v)*ch, cw-2, ch*v);
                fill();
                closePath();
            }
        }
    }
}


END_NAMESPACE_DISTRHO