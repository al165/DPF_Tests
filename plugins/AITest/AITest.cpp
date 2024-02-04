#include "AITest.hpp"



START_NAMESPACE_DISTRHO

AITest::AITest() : Plugin(kParameterCount, 0, 1),
    fThreshold(0.7)
{

    try {
        module = torch::jit::load("/home/arran/Projects/WAIVE/03_Plugin/DPFTutorial/models/beat_model_test.zip");
        has_model_ = true;

    } catch (const c10::Error& e) {
        std::cerr << "error loading model\n";
        std::cerr << e.msg() << "\n";
    }

    fSixteenth = 0.0f;
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
        case kGenerate:
            parameter.name = "Generate New";
            parameter.symbol = "generate_new";
            parameter.hints = kParameterIsOutput;
            break;
        case kSixteenth:
            parameter.name = "Sixteenth";
            parameter.name = "sixteenth";
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 16.0f;
            parameter.ranges.def = 0.0f;
            parameter.hints = kParameterIsOutput | kParameterIsInteger | kParameterIsAutomatable;
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
    printf("AITest::setParameterValue: %d %.2f\n", index, value);
    switch(index)
    {
        case kGenerate:
            generateNew();
            break;
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
    uint32_t,                    // size of block to process
    const MidiEvent *midiEvents, // MIDI pointer
    uint32_t midiEventCount      // Number of MIDI events in block
)
{
    const TimePosition& timePos(getTimePosition());

    if (timePos.bbt.valid)
    {
        float tick = timePos.bbt.tick;
        float beat = timePos.bbt.beat - 1.0f;
        float tpb = timePos.bbt.ticksPerBeat;

        fSixteenth = (beat + (tick / tpb)) * 4.0f;
    } else {
        fSixteenth = 0.0f;
    }

    for (uint32_t i=0; i<midiEventCount; ++i) 
    {
        writeMidiEvent(midiEvents[i]);

        if((int) fSixteenth != sixteenthProcessed)
        {
            sixteenthProcessed = (int) fSixteenth;

            if(pattern[0][sixteenthProcessed])
            {
                
            }
        }
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


Plugin *createPlugin()
{
    return new AITest();
}

END_NAMESPACE_DISTRHO