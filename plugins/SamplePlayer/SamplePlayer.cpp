#include "SamplePlayer.hpp"


START_NAMESPACE_DISTRHO

SamplePlayer::SamplePlayer() : Plugin(kParameterCount, 0, 2)
{
    fAmp = 1.0f;
    loadedSample = false;
    fMidiNumber = 60;

    sampleRate = getSampleRate();
};

void SamplePlayer::initParameter(uint32_t index, Parameter &parameter)
{
    switch(index)
    {
        case kAmp:
            parameter.name = "Amp";
            parameter.symbol = "amp";
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 2.0f;
            parameter.ranges.def = 1.0f;
            parameter.hints = kParameterIsAutomatable;
            break;
        case kMidiNote:
            parameter.name = "Midi Note";
            parameter.symbol = "midi_note";
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 127.0f;
            parameter.ranges.def = 60.0f;
            parameter.hints = kParameterIsInteger;
            break;
        case kSampleLoaded:
            parameter.name = "Sample Loaded";
            parameter.symbol = "sample_loaded";
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.0f;
            parameter.ranges.def = 0.0f;
            parameter.hints = kParameterIsBoolean | kParameterIsOutput;
            break;
        default:
            break;
    }
};

float SamplePlayer::getParameterValue(uint32_t index) const
{
    float val = 0.0f;
    switch(index)
    {
        case kAmp:
            val = fAmp;
            break;
        case kMidiNote:
            val = fMidiNumber;
            break;
        case kSampleLoaded:
            val = loadedSample;
            break;
        default:
            break;
    }

    return val;
};

void SamplePlayer::setParameterValue(uint32_t index, float value) 
{
    switch(index)
    {
        case kAmp:
            fAmp = value;
            break;
        case kMidiNote:
            fMidiNumber = value;
            break;
        case kSampleLoaded:
            break;
        default:
            break;
    }
};

void SamplePlayer::setState(const char *key, const char *value)
{
    if(strcmp(key, "ui_sample_loaded") == 0)
    {
        loadedSample = false;
    }

    else if(strcmp(key, "filepath") == 0)
    {
        path = std::string(value);
        // loadSample(path);
    }
};

String SamplePlayer::getState(const char *key) const
{
    String retString = String("undefined state");
    if(strcmp(key, "filepath") == 0)
    {
        retString = path.c_str();
    }
    else if(strcmp(key, "ui_sample_loaded") == 0){
        retString = "ui_sample_loaded yes/no";
    }

    return retString;
};

void SamplePlayer::initState(unsigned int index, String &stateKey, String &defaultStateValue)
{
    switch(index)
    {
        case 0:
            stateKey = "filepath";
            defaultStateValue = "empty";
            break;
        case 1:
            stateKey = "ui_sample_loaded";
            defaultStateValue = "false";
            break;
        default:
            printf("initState unknown: %i\n", index);
            break;
    }
};

int SamplePlayer::loadSample(const char *fp)
{
    printf("loadSample %s", fp);

    return 0;
};


void SamplePlayer::run(
    const float **,              // incoming audio
    float **outputs,             // outgoing audio
    uint32_t frames,             // size of block to process
    const MidiEvent *midiEvents, // MIDI pointer
    uint32_t midiEventCount      // Number of MIDI events in block
)
{
    std::fill_n(outputs[0], frames, 0.0f);
    std::fill_n(outputs[1], frames, 0.0f);
    return;
};


Plugin *createPlugin()
{
    return new SamplePlayer();
};


END_NAMESPACE_DISTRHO