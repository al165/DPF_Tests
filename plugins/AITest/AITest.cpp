#include "AITest.hpp"

START_NAMESPACE_DISTRHO

AITest::AITest() : Plugin(kParameterCount, 0, 1),
    fThreshold(0.7),
    fEmbedX(0.0),
    fEmbedY(0.0)
{

    std::stringstream model_stream;

    try {
        model_stream.write((char *) beat_model_5x16_zip, beat_model_5x16_zip_len);
        beat_model = torch::jit::load(model_stream);

        model_stream.str("");
        model_stream.write((char *) grid_embedder_2d_model_zip, grid_embedder_2d_model_zip_len);
        grid_embedding_2d_model = torch::jit::load(model_stream);

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
            parameter.symbol = "sixteenth";
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 16.0f;
            parameter.ranges.def = 0.0f;
            parameter.hints = kParameterIsOutput | kParameterIsInteger;
            break;
        case kEmbedX:
            parameter.name = "MapX";
            parameter.symbol = "map_x";
            parameter.ranges.min = -8.0f;
            parameter.ranges.max = 5.0f;
            parameter.ranges.def = 0.0f;
            parameter.hints = kParameterIsAutomatable;
        case kEmbedY:
            parameter.name = "MapY";
            parameter.symbol = "map_y";
            parameter.ranges.min = -2.5f;
            parameter.ranges.max = 5.0f;
            parameter.ranges.def = 0.0f;
            parameter.hints = kParameterIsAutomatable;
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
        case kEmbedX:
            val = fEmbedX;
            break;
        case kEmbedY:
            val = fEmbedY;
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
        case kEmbedX:
            fEmbedX = value;
            generateFromEmbedding();
            break;
        case kEmbedY:
            fEmbedY = value;
            generateFromEmbedding();
            break;
        default:
            break;
    }
}


void AITest::setState(const char *key, const char *value)
{
    if (std::strcmp(key, "beat_grid") == 0)
    {
        for(int i=0; i<INS; i++)
        {
            for(int j=0; j<GS; j++)
            {
                pattern[i][j] = value[i*16+j];
            }
        }
    }
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

        for(uint32_t i=0; i<INS; i++)
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

    for(uint32_t s = 0; s < numFrames; s++)
    {
        if(loopFrame % (int) samplesPer16th == 0)
        {
            MidiEvent me;
            me.size = 3;
            me.frame = s;
            
            for(uint32_t i=0; i<INS; i++)
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

    // TODO: make random fEmbedX and Y

    generateFromEmbedding();
}

void AITest::generateFromEmbedding(){
    if(!has_model_) return;

    std::vector<torch::jit::IValue> inputs;
    inputs.push_back(torch::tensor({fEmbedX, fEmbedY}));

    std::cout << "2d inputs:\n" << inputs << "\n";

    embedding = grid_embedding_2d_model.forward(inputs).toTensor();

    std::vector<torch::jit::IValue> Z;
    Z.push_back(embedding);

    gridOutput = beat_model.forward(Z).toTensor().reshape({INS, GS});
    std::cout << gridOutput << "\n";

    updatePattern();
}

void AITest::updatePattern(){
    for(int i=0; i<INS; i++)
    {
        for(int j=0; j<GS; j++)
        {
            float val = std::min(1.0f, std::max(0.0f, gridOutput.index({i, j}).item<float>()));
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