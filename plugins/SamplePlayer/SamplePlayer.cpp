#include "SamplePlayer.hpp"


START_NAMESPACE_DISTRHO

SamplePlayer::SamplePlayer() : Plugin(kParameterCount, 0, 2)
{
    fAmp = 1.0f;
    loadedSample = false;
    fMidiNumber = 60;

    sampleRate = getSampleRate();
    samplePtr = 0;
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
            parameter.hints = kParameterIsInteger | kParameterIsAutomatable;
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
        loadSample(path.c_str());
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
    printf("loadSample %s\n", fp);

    SndfileHandle fileHandle(fp);

    int newSampleLength = fileHandle.frames() - 1;
    
    if(newSampleLength <= 0){
        path = "--";
        return 0;
    }

    sampleLength = newSampleLength;
    waveform.resize(0);
    int channels = fileHandle.channels();
    waveform.resize(sampleLength * channels);
    fileHandle.read(&waveform.at(0), sampleLength * channels);

    printf("sampleLength: %d channels: %d\n ", sampleLength, channels);
    loadedSample = true;

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
    if(!loadedSample)
    {
        std::fill_n(outputs[0], frames, 0.0f);
        std::fill_n(outputs[1], frames, 0.0f);
        return;
    }

    float* const outL = outputs[0]; // output ports , stereo
    float* const outR = outputs[1];

    uint32_t framesDone = 0;
    while(framesDone < frames)
    {
        outL[framesDone] = waveform[samplePtr*2] * fAmp;
        outR[framesDone] = waveform[samplePtr*2+1] * fAmp;

        framesDone++;
        samplePtr = (samplePtr + 1) % sampleLength;
    }
};


Plugin *createPlugin()
{
    return new SamplePlayer();
};


END_NAMESPACE_DISTRHO