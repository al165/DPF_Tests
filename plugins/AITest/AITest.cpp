#include "AITest.hpp"

START_NAMESPACE_DISTRHO

AITest::AITest() : Plugin(kParameterCount, 0, 1),
    fThreshold(0.7)
{

    std::stringstream model_stream;
    model_stream.write((char *) beat_model_test_zip, beat_model_test_zip_len);

    try {
        module = torch::jit::load(model_stream);
        has_model_ = true;

    } catch (const c10::Error& e) {
        std::cerr << "error loading model\n";
        std::cerr << e.msg() << "\n";
    }

    fSixteenth = 0.0f;
    sixteenthProcessed = 0;
    sampleRate = getSampleRate();
    generateNew();
};


void AITest::initParameter(uint32_t index, Parameter &parameter)
{
    switch(index)
    {
        case kThreshold:
            parameter.name = "Threshold";
            parameter.symbol = "threshold";
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.0f;
            parameter.ranges.def = 0.7f;
            parameter.hints = kParameterIsAutomatable;
            break;
        case kSixteenth:
            parameter.name = "Sixteenth";
            parameter.name = "sixteenth";
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 16.0f;
            parameter.ranges.def = 0.0f;
            parameter.hints = kParameterIsOutput | kParameterIsInteger;
            break;
        default:
            break;
    }
}

float AITest::getParameterValue(uint32_t index) const
{
    float val = 0.0f;
    switch(index)
    {
        case kThreshold:
            val = fThreshold;
            break;
        case kSixteenth:
            val = fSixteenth;
            break;
        default:
            break;
    }

    return val;
}

void AITest::setParameterValue(uint32_t index, float value)
{
    switch(index)
    {
        case kThreshold:
            fThreshold = value;
            updatePattern();
            break;
        default:
            break;
    }
}


void AITest::setState(const char *key, const char *value)
{
    if (std::strcmp(key, "beat_grid") == 0)
    {
        for(int i=0; i<3; i++)
        {
            for(int j=0; j<16; j++)
            {
                pattern[i][j] = value[i*16+j];
            }
        }
    }
}

String AITest::getState(const char *key) const
{
    printf("getState()\n");
    String retString = String("undefined state");
    return retString;
}

void AITest::initState(unsigned int index, String &stateKey, String &defaultStateValue)
{
    printf(" == initState\n");
    switch(index)
    {
        case 0:
            printf(" init beat_grid\n");
            stateKey = "beat_grid";
            defaultStateValue = "";
            break;
        default:
            break;
    }
}


void AITest::run(
    const float **,              // incoming audio
    float **,                    // outgoing audio
    uint32_t numFrames,          // size of block to process
    const MidiEvent *midiEvents, // MIDI pointer
    uint32_t midiEventCount      // Number of MIDI events in block
)
{
    const TimePosition& timePos(getTimePosition());


    for(uint32_t i=0; i<midiEventCount; ++i) 
    {
        MidiEvent me = midiEvents[i];
        writeMidiEvent(me);
        printf("MidiEvent: size %d frame %d data %08b %08b %08b\n", me.size, me.frame, me.data[0], me.data[1], me.data[2]);
    }

    static bool wasPlaying = false;
    static uint32_t loopFrame = 0;

    if(!timePos.playing && wasPlaying)
    {
        fSixteenth = 0.0f;
        sixteenthProcessed = 0;
        loopFrame = 0;

        MidiEvent me;
        me.size = 3;
        me.frame = 0;

        for(uint32_t i=0; i<3; i++)
        {
            if(triggered[i])
            {
                triggered[i] = 0;
                // MIDI NoteOff
                me.data[0] = 0x80;
                me.data[1] = MidiMap[i];
                me.data[2] = 0;

                writeMidiEvent(me);
            }
        }
    }

    wasPlaying = timePos.playing;

    if(!timePos.playing) return;

    if(!timePos.bbt.valid)
    {
        fSixteenth = 0.0f;
        sixteenthProcessed = 0;
        triggered[0] = 0;
        triggered[1] = 0;
        triggered[2] = 0;
        loopFrame = 0;
        return;
    }

    float tick = timePos.bbt.tick;
    float beat = timePos.bbt.beat - 1.0f;
    float tpb = timePos.bbt.ticksPerBeat;

    double samplesPerBeat = (60.0 * sampleRate) / timePos.bbt.beatsPerMinute;
    double samplesPerBar = samplesPerBeat * 4.0;
    double samplesPer16th = samplesPerBeat * 0.25;
    double samplesPerTick = samplesPerBeat / tpb;
    loopFrame = (int) (samplesPerTick * tick);

    fSixteenth = (beat + (tick / tpb)) * 4.0f;
    // sixteenthProcessed = (int) fSixteenth;

    for(uint32_t s = 0; s < numFrames; s++)
    {
        if(loopFrame % (int) samplesPer16th == 0)
        {
            MidiEvent me;
            me.size = 3;
            me.frame = s;
            
            for(uint32_t i=0; i<3; i++)
            {
                if(triggered[i])
                {
                    triggered[i] = 0;
                    // MIDI NoteOff
                    me.data[0] = 0x80;
                    me.data[1] = MidiMap[i];
                    me.data[2] = 0;

                    writeMidiEvent(me);
                }

                if(pattern[i][sixteenthProcessed] > 0)
                {
                    triggered[i] = 1;

                    me.data[0] = 0x90;
                    me.data[1] = MidiMap[i];
                    me.data[2] = pattern[i][sixteenthProcessed];

                    writeMidiEvent(me);
                }
            }

            sixteenthProcessed = (sixteenthProcessed + 1) % 16;
        }

        loopFrame = (loopFrame+1) % (int) samplesPerBar;
    }

};


void AITest::generateNew(){
    if(!has_model_) return;

    std::vector<torch::jit::IValue> inputs;
    inputs.push_back(torch::randn({24}));

    output = module.forward(inputs).toTensor().reshape({3, 16});
    std::cout << output << "\n";

    updatePattern();
}

void AITest::updatePattern(){
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<16; j++)
        {
            float val = std::min(1.0f, std::max(0.0f, output.index({i, j}).item<float>()));
            char c = 128 * (val > fThreshold ? val : 0.0f);

            pattern[i][j] = c;
        }
    }
}

void AITest::sampleRateChanged(double newSampleRate)
{
    sampleRate = newSampleRate;
}


Plugin *createPlugin()
{
    return new AITest();
}

END_NAMESPACE_DISTRHO