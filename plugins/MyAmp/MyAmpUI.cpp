#include "DistrhoUI.hpp"

START_NAMESPACE_DISTRHO

/**
  We need the Color class from DGL.
 */
using DGL_NAMESPACE::Color;

class MyAmpUI : public UI
{
    public:
        MyAmpUI() 
            :   UI(128, 512),
                fColor(93, 231, 61),
                fGainL(0.0f),
                fGainR(0.0f),
                fOutLeft(0.0f),
                fOutRight(0.0f)
        {
            setGeometryConstraints(32, 128, false);
        }

    protected:

        void parameterChanged(uint32_t index, float value) override
        {
            switch(index){
                case kGainL:
                    if (value < 0.001f) value = 0.0f;
                    else if (value > 1.999f) value = 2.0f;

                    if(fGainL != value){
                        fGainL = value;
                        repaint();
                    }
                    break;
                case kGainR:
                    if (value < 0.001f) value = 0.0f;
                    else if (value > 1.999f) value = 2.0f;

                    if(fGainR != value){
                        fGainR = value;
                        repaint();
                    }
                    break;
                case kOutL:
                    if (value < 0.001f) value = 0.0f;
                    else if (value > 0.999f) value = 1.0f;

                    if(fOutLeft != value){
                        fOutLeft = value;
                        repaint();
                    }
                    break;
                case kOutR:
                    if (value < 0.001f) value = 0.0f;
                    else if (value > 0.999f) value = 1.0f;
                    
                    if(fOutRight != value){
                        fOutRight = value;
                        repaint();
                    }
                    break;
            }
        }

        void stateChanged(const char*, const char*) override
        {
            // nothing here
        }

        void onNanoDisplay() override
        {
            static const Color kColorBlack(0, 0, 0);
            static const Color kColorRed(255, 0, 0);

            const float outLeft(fOutLeft);
            const float outRight(fOutRight);
            const float gainLeft(fGainL);
            const float gainRight(fGainR);

            const float halfWidth = static_cast<float>(getWidth())/2.0f;

            beginPath();
            rect(0.0f, 0.0f, halfWidth, getHeight());
            fillColor(kColorBlack);
            fill();
            closePath();

            beginPath();
            rect(0.0f, getHeight()*(1.0f-outLeft), halfWidth, getHeight()*outLeft);
            fillColor(fColor);
            fill();
            closePath();

            beginPath();
            moveTo(0, getHeight()*(1.0f-0.5f*gainLeft));
            lineTo(halfWidth, getHeight()*(1.0f-0.5f*gainLeft));
            strokeWidth(2);
            strokeColor(kColorRed);
            stroke();
            closePath();

            beginPath();
            rect(halfWidth, 0.0f, halfWidth, getHeight());
            fillColor(kColorBlack);
            fill();
            closePath();

            beginPath();
            rect(halfWidth, getHeight()*(1.0f-outRight), halfWidth, getHeight()*outRight);
            fillColor(fColor);
            fill();
            closePath();

            beginPath();
            moveTo(halfWidth, getHeight()*(1.0f-0.5f*gainRight));
            lineTo(getWidth(), getHeight()*(1.0f-0.5f*gainRight));
            strokeWidth(2);
            strokeColor(kColorRed);
            stroke();
            closePath();
        }

        bool onMouse(const MouseEvent& ev) override
        {
            // Test for left-clicked + pressed first.
            if (ev.button != 1 || ! ev.press){
                dragging = false;
                return false;
            }

            dragging = true;

            const float halfWidth = static_cast<float>(getWidth())/2.0f;
            float y = ev.pos.getY();
            float gain = 2.0f * (1.0f - y / (float)getHeight()); 

            if(ev.pos.getX() < halfWidth){
                editParameter(kGainL, true);
                setParameterValue(kGainL, gain);
                editParameter(kGainL, false);
            } else {
                editParameter(kGainR, true);
                setParameterValue(kGainR, gain);
                editParameter(kGainR, false);
            }
            repaint();
            return true;
        }

        bool onMotion(const MouseEvent& ev)
        {
            if(!dragging) return false;

            

            return true;
        }

    private:
        Color fColor;
        float fGainL, fGainR, fOutLeft, fOutRight;

        bool dragging = false;

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyAmpUI)
};

UI* createUI()
{
    return new MyAmpUI();
}


END_NAMESPACE_DISTRHO