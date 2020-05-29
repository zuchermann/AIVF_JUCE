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
    forcedinline float getNextSample() noexcept;
    forcedinline void updateAngle() noexcept;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    AudioSourcePlayer* getSource();
private:
    float currentAngle = 0.0f, angleDelta = 0.0f, level = 0.0f;
    AudioSourcePlayer audioSource;
    float sr;
};
