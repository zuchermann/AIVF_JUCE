/*
  ==============================================================================

    Melody2.h
    Created: 12 Jun 2020 8:02:01pm
    Author:  YN

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Melody2 : AudioSource{
public:
    Melody2();
    ~Melody2() override;
    void releaseResources () override;
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void setFrequency (float frequency);
    void setSuccess (float success);
    void tick (int i);
    void set_ts (std::vector<int> ts, double len);
    void start();
    void generateMelody();
    void stop();
    float midiToFreq(float midi_val);
    forcedinline float getNextSample() noexcept;
    forcedinline void updateAngle() noexcept;
    forcedinline void updateEnv() noexcept;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    AudioSourcePlayer* getSource();
private:
    float twoPi = MathConstants<float>::twoPi;
    float currentAngle = 0.0f, angleDelta = 0.0f, level = 0.4f;
    float env_freq = 2.0f, env = 0.0f, env_delta = 0.0f; //filter period of .5 seconds
    std::vector<double> lengths;// = {}
    std::vector<float> pitches;
    std::vector<int> melody_events;
    std::vector<double> rhythm_error;
    std::vector<float> detune_dirs = {-1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f};
    AudioSourcePlayer audioSource;
    std::vector<int> t_events;
    int next_to_play = 0;
    int base_pitch;
    double vid_length;
    double beats_per_vid = 4;
    float succ = 0.0;
    //IIRFilter *filter_0;
    //IIRFilter *filter_1;
    //int t_offset;
    //float Q = 10.0f;
    //Random random;
    //std::vector<float> filter_freqs;
    float sr;
};
