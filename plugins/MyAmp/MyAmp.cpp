#include "DistrhoPlugin.hpp"

START_NAMESPACE_DISTRHO


class MyAmp : public Plugin {
    public:
        MyAmp() 
            : Plugin(kParameterCount, 0, 0), // n parameters, 0 programs, 0 states
              gainL(1.0f),
              gainR(1.0f),
              fOutLeft(0.0f),
              fOutRight(0.0f)
            {

            }

    protected:
        const char *getLabel() const override { return "MyAmp"; }
        const char *getDescription() const override {
            return "Simple amp plugin.";
        }
        const char *getMaker() const override { return "REIS0"; }
        const char *getLicense() const override { return "MIT"; }
        uint32_t getVersion() const override { return d_version(1,1,0); }
        int64_t getUniqueId() const override { 
            return d_cconst('M','A','D','T'); 
        }

        void initAudioPort(bool input, uint32_t index, AudioPort& port) override {
            // treat meter audio ports as stereo
            port.groupId = kPortGroupStereo;

            // everything else is as default
            Plugin::initAudioPort(input, index, port);
        }

        void initParameter (uint32_t index, Parameter& parameter) override {
            parameter.hints = kParameterIsAutomatable;
            switch(index){
                case kGainL:
                    parameter.name = "Gain Left";
                    parameter.symbol = "gainL";
                    parameter.ranges.def = 1.0f;
                    parameter.ranges.min = 0.0f;
                    parameter.ranges.max = 2.0f;
                    break;
                case kGainR:
                    parameter.name = "Gain Right";
                    parameter.symbol = "gainR";
                    parameter.ranges.def = 1.0f;
                    parameter.ranges.min = 0.0f;
                    parameter.ranges.max = 2.0f;
                    break;
                case kOutL:
                    parameter.hints  = kParameterIsAutomatable|kParameterIsOutput;
                    parameter.name = "Out Left";
                    parameter.symbol = "out_left";
                    break;
                case kOutR:
                    parameter.hints  = kParameterIsAutomatable|kParameterIsOutput;
                    parameter.name = "Out Right";
                    parameter.symbol = "out_right";
                    break;
                default:
                    break;
            }
        }

        float getParameterValue(uint32_t index) const override {
            switch(index){
                case kGainL:
                    return gainL;
                case kGainR:
                    return gainR;
                case kOutL:
                    return fOutLeft;
                case kOutR:
                    return fOutRight;
                default:
                    return 0.0f;
            }
        }

        void setParameterValue(uint32_t index, float value) override {
            switch(index){
                case kGainL:
                    gainL = value;
                    break;
                case kGainR:
                    gainR = value;
                    break;
                case kOutL:
                    fOutLeft = value;
                    break;
                case kOutR:
                    fOutRight = value;
                    break;
                default:
                    break;
            }
        }

        void run(const float **inputs, float **outputs, uint32_t frames) override {
            const float* inL = inputs[0]; // input ports , stereo
            const float* inR = inputs[1];
            float* const outL = outputs[0]; // output ports , stereo
            float* const outR = outputs[1];

            uint32_t framesDone = 0;
            float tempL = 0.0f;
            float tempR = 0.0f;

            while(framesDone < frames){
                outL[framesDone] = inL[framesDone]*gainL;
                outR[framesDone] = inR[framesDone]*gainR;

                tempL = std::max(tempL, std::abs(outL[framesDone]));
                tempR = std::max(tempR, std::abs(outR[framesDone]));

                framesDone++;
            }

            fOutLeft = std::min(1.0f, tempL);
            fOutRight = std::min(1.0f, tempR);
        }

    private:
        float gainL, gainR, fOutLeft, fOutRight;

        DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyAmp);
};

Plugin* createPlugin(){ return new MyAmp(); }

END_NAMESPACE_DISTRHO