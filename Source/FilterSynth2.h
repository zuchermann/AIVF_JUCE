/*
  ==============================================================================

    FilterSynth2.h
    Created: 5 Jun 2020 7:56:26pm
    Author:  YN

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class FilterSynth2 : AudioSource{
public:
    FilterSynth2();
    ~FilterSynth2() override;
    void releaseResources () override;
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void setFrequency (float frequency);
    void setLevel (float lev);
    void setHarmDist(float dist);
    void setNoise(float noise);
    void setClip(float clip_amt);
    void event (int i);
    void set_ts (std::vector<int> ts);
    void start();
    void stop();
    forcedinline float getNextSample() noexcept;
    forcedinline void updateAngle() noexcept;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    AudioSourcePlayer* getSource();
private:
    float twoPi = MathConstants<float>::twoPi;
    float clip = 1.0;
    float harm_distortion = 0.0;
    float freq = 0.0f;
    float noise_lev = 0.5;
    int num_sines = 100;
    std::vector<float> currentAngles, angleDeltas, amps;
    float level = 0.0f;
    AudioSourcePlayer audioSource;
    IIRFilter *filter_0;
    IIRFilter *filter_1;
    int t_offset;
    float Q = 10.0f;
    Random random;
    std::vector<float> filter_freqs;
    float sr;
};
