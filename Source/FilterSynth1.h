/*
  ==============================================================================

    filterSynth.h
    Created: 28 May 2020 7:24:52pm
    Author:  YN

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class FilterSynth1 : AudioSource{
public:
    FilterSynth1();
    ~FilterSynth1() override;
    void releaseResources () override;
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void setFrequency (float frequency);
    void setLevel (float lev);
    void event (int i);
    void set_ts (std::vector<int> ts);
    void start();
    void stop();
    forcedinline float getNextSample() noexcept;
    forcedinline void updateAngle() noexcept;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    AudioSourcePlayer* getSource();
private:
    float currentAngle = 0.0f, angleDelta = 0.0f, level = 0.0f;
    AudioSourcePlayer audioSource;
    IIRFilter *filter_0;
    IIRFilter *filter_1;
    int t_offset;
    float Q = 10.0f;
    Random random;
    std::vector<float> filter_freqs;
    float sr;
};
