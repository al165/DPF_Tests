#include "AITest.hpp"



START_NAMESPACE_DISTRHO

AITest::AITest() : Plugin(0, 0, 0)
{
    torch::Tensor tensor = torch::eye(3);
    std::cout << tensor << std::endl;

    try {
        module = torch::jit::load("/home/arran/Projects/WAIVE/03_Plugin/DPFTutorial/models/beat_model_test.zip");

        std::vector<torch::jit::IValue> inputs;
        inputs.push_back(torch::randn({24}));

        at::Tensor output = module.forward(inputs).toTensor();
        std::cout << output << "\n";
    } catch (const c10::Error& e) {
        std::cerr << "error loading model\n";
        std::cerr << e.msg() << "\n";
    }

    
};


void AITest::initParameter(uint32_t index, Parameter &parameter)
{
    switch(index)
    {
        default:
            break;
    }
}

float AITest::getParameterValue(uint32_t index) const
{
    float val = 0.0f;
    switch(index)
    {
        default:
            break;
    }

    return val;
}

void AITest::setParameterValue(uint32_t index, float value)
{
    switch(index)
    {
        default:
            break;
    }
}

/**
void AITest::setState(const char *key, const char *value)
{
}

String AITest::getState(const char *key) const
{
    String retString = String("undefined state");
    return retString;
}

void AITest::initState(unsigned int index, String &stateKey, String &defaultStateValue)
{
    switch(index)
    {
        default:
            break;
    }
}
**/

void AITest::run(
    const float **,              // incoming audio
    float **,                    // outgoing audio
    uint32_t,             // size of block to process
    const MidiEvent *midiEvents, // MIDI pointer
    uint32_t midiEventCount      // Number of MIDI events in block
)
{

};

Plugin *createPlugin()
{
    return new AITest();
}

END_NAMESPACE_DISTRHO