#ifndef SAMPLEPLAYER_HPP_INCLUDED
#define SAMPLEPLAYER_HPP_INCLUDED

#include "DistrhoPlugin.hpp"
#include "SamplePlayerParams.h"
#include <string>
#include <vector>
#include <iostream>
#include <sndfile.hh>

START_NAMESPACE_DISTRHO

class SamplePlayer : public Plugin
{
public:
    SamplePlayer();

protected:
    // --- Info -------------------
    const char *getLabel() const noexcept override
    {
        return "SamplePlayer";
    }

    const char *getDescription() const override
    {
        return "Basic sample player learned from Drops";
    }

    const char *getMaker() const noexcept override
    {
        return "arnlyn";
    }

    const char *getHomePage() const override
    {
        return "https://github.com/arnlyn/sampleplayer";
    }

    const char *getLicense() const noexcept override
    {
        return "GPL V3";
    }

    uint32_t getVersion() const noexcept override
    {
        return d_version(1, 0, 0);
    }

    int64_t getUniqueId() const noexcept override
    {
        return d_cconst('a', 'r', 'n', 'S');
    }


    // --- Init -------------------
    void initParameter(uint32_t index, Parameter &parameter) override;
    void setState(const char *key, const char *value) override;
    String getState(const char *key) const override;
    void initState(unsigned int, String &, String &) override;

    // --- Internal data ----------
    float getParameterValue(uint32_t index) const override;
    void setParameterValue(uint32_t index, float value) override;

    // --- Process ----------------
    void run(const float **, float **outputs, uint32_t frames, const MidiEvent *midiEvents, uint32_t midiEventCount) override;

    // -------------------------------------------------------------------

private:
    int loadSample(const char *fp);

    float fAmp;
    bool loadedSample;
    int fMidiNumber;

    std::string path;
    double sampleRate;
    std::vector<float> waveform;
    int sampleLength;
    int samplePtr;

    friend class SamplePlayerUI;
};

END_NAMESPACE_DISTRHO

#endif