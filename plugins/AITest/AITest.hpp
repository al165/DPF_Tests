#ifndef AITEST_HPP_INCLUDED
#define AITEST_HPP_INCLUDED

#include "DistrhoPlugin.hpp"
#include "AITestParams.h"
#include <string>
#include <iostream>

#include <torch/script.h>

START_NAMESPACE_DISTRHO

class AITest : public Plugin
{
public:
    AITest();

protected:
    const char *getLabel() const noexcept override
    {
        return "AITest";
    }

    const char *getDescription() const override
    {
        return "Test of using PyTorch";
    }

    const char *getMaker() const noexcept override
    {
        return "arnlyn";
    }

    const char *getHomePage() const override
    {
        return "https://github.com/arnlyn/AITest";
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
        return d_cconst('a', 'r', 'n', 'A');
    }

    void initParameter(uint32_t index, Parameter &parameter) override;
    void setState(const char *key, const char *value) override;
    String getState(const char *key) const override;
    void initState(unsigned int, String &, String &) override;

    // --- Internal data ----------
    float getParameterValue(uint32_t index) const override;
    void setParameterValue(uint32_t index, float value) override;

    // --- Process ----------------
    // void activate() override;
    void run(const float **, float **, uint32_t, const MidiEvent *midiEvents, uint32_t midiEventCount) override;

private:
    bool has_model_;
    float fThreshold, fSixteenth;

    torch::jit::script::Module module;
    at::Tensor output;

    uint8_t pattern[3][16];

    void generateNew();
    void updatePattern();

    int sixteenthProcessed = -1; // the last 16th note processed;

    // AITestUI ui;
    friend class AITestUI;

};


END_NAMESPACE_DISTRHO

#endif