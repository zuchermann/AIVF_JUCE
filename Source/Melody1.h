/*
  ==============================================================================

    Melody1.h
    Created: 10 Jun 2020 4:52:00pm
    Author:  YN

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Melody1 : AudioSource{
public:
    Melody1();
    ~Melody1() override;
    void releaseResources () override;
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void setFrequency (float frequency);
    void setSuccess (float success);
    void event (int i);
    void set_ts (std::vector<int> ts);
    void start();
    void stop();
    float midiToFreq(float midi_val);
    forcedinline float getNextSample() noexcept;
    forcedinline void updateAngle() noexcept;
    forcedinline void updateEnv() noexcept;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    AudioSourcePlayer* getSource();
private:
    float twoPi = MathConstants<float>::twoPi;
    float currentAngle = 0.0f, angleDelta = 0.0f, level = 0.0f;
    float env_freq = 2.0f, env = 0.0f, env_delta = 0.0f; //filter period of .5 seconds
    AudioSourcePlayer audioSource;
    int base_pitch;
    float succ = 0.0;
    //IIRFilter *filter_0;
    //IIRFilter *filter_1;
    //int t_offset;
    //float Q = 10.0f;
    //Random random;
    //std::vector<float> filter_freqs;
    float sr;
};
