#ifndef AITESTPARAMS_HPP_INCLUDED
#define AITESTPARAMS_HPP_INCLUDED

enum Parameters {
    kThreshold,
    kSixteenth,
    kEmbedX,
    kEmbedY,
    kParameterCount
};

enum MidiDrumNotes {
    KD = 0x24,  // 36
    SD = 0x26,  // 38
    HH = 0x2A,  // 42
    CL = 0x27,  // 39
    TH = 0x2B   // 43
};

const uint8_t MidiMap[5] = {KD, SD, HH, CL, TH};

#endif