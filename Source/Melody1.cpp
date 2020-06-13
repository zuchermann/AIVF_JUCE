/*
  ==============================================================================

    Melody1.cpp
    Created: 10 Jun 2020 4:52:00pm
    Author:  YN

  ==============================================================================
*/

#include "Melody1.h"
#include "MusicUtilities.h"

Melody1::Melody1() {
    //filter_freqs = {1500.0f, 2000.0f, 300.0f, 3000.0f, 1000.0f, 200.0f, 500.0f, 1000.0f};
}

Melody1::~Melody1() {
}

void Melody1::releaseResources() {
}

float Melody1::midiToFreq(float midi_val) {
    return  440.0 * (std::pow (2.0f, (midi_val - 69.0f) / 12.0f));
}

void Melody1::setSuccess(float success) {
    float adjusted_succ = (success - 1.0f) * -1.0f;
    succ = adjusted_succ;
}

void Melody1::event(int i){
    std::vector<int> scale_degrees = {0, 2, 4, 1, 6, 3, 5, 7};
    int pitch_class = MusicUtilities::major_degree_to_pitchclass(scale_degrees[i]);
    int scale_degree = scale_degrees[i];
    std::vector<float> detune_dirs = {1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f};
    float detune_dir = detune_dirs[scale_degree] * 1.0f;
    float this_pitch = (detune_dir * succ) + ((float) pitch_class) + ((float) base_pitch);
    //std::cout << this_pitch << "\n";
    float freq = midiToFreq(this_pitch);
    //std::cout << freq << "\n";
    setFrequency(freq);
    //std::cout << i << "\n";
    //int i_offset = (i + t_offset) % ((int) filter_freqs.size());
    //float freq = filter_freqs[i_offset];
    //filter_0->setCoefficients(IIRCoefficients::makeLowPass(sr, freq, Q));
    //filter_1->setCoefficients(IIRCoefficients::makeLowPass(sr, freq, Q));
}

void Melody1::setFrequency (float frequency)
{
    auto cyclesPerSample = frequency / sr;
    auto envPerSample = env_freq / sr;
    angleDelta = cyclesPerSample;
    currentAngle = 0.0f;
    env = 1.0;
    env_delta = envPerSample;
}

AudioSourcePlayer* Melody1::getSource() {
    audioSource.setSource(this);
    return &audioSource;
}

forcedinline float Melody1::getNextSample() noexcept
{
    //float noise = random.nextFloat() * 0.5f;
    float current_wave = std::pow (std::abs (std::sin(currentAngle)), (1.0f + (std::pow (env, 2) * 100.0f * (succ * 10.0f))));
    current_wave *= 2.0f;
    current_wave -= 1.0f;
    auto currentSample = current_wave * env;
    updateAngle();
    updateEnv();
    return currentSample;
}

void Melody1::prepareToPlay (int, double sampleRate) {
    sr = sampleRate;
    //filter_0 = new IIRFilter();
    //filter_1 = new IIRFilter();
    //int num_freqs = (int) filter_freqs.size();
    //int offset_i = ((num_freqs - 1) + t_offset) % num_freqs;
    //filter_0->setCoefficients(IIRCoefficients::makeLowPass (sr, filter_freqs[offset_i], Q));
    //filter_1->setCoefficients(IIRCoefficients::makeLowPass (sr, filter_freqs[offset_i], Q));
}

forcedinline void Melody1::updateAngle() noexcept
{
    currentAngle += angleDelta;
    if (currentAngle >= twoPi)
        currentAngle -= twoPi;
}

forcedinline void Melody1::updateEnv() noexcept {
    env -= env_delta;
    if (env < 0.0f)
        env = 0.0f;
}

void Melody1::set_ts (std::vector<int> ts){
    //int num_total = (int) filter_freqs.size();
    //t_offset = ts[0] % num_total;
    base_pitch = 60 + ts[2] % 12;
}

void Melody1::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    auto* leftBuffer  = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample); // [7]
    auto* rightBuffer = bufferToFill.buffer->getWritePointer (1, bufferToFill.startSample);
    
 
    bufferToFill.clearActiveBufferRegion();
    
    for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
    {
        auto levelSample = getNextSample();       // [9]
        
        leftBuffer[sample]  += levelSample;                           // [10]
        rightBuffer[sample] += levelSample;
    }
    
    //filter_0->processSamples(leftBuffer, bufferToFill.numSamples);
    //filter_1->processSamples(rightBuffer, bufferToFill.numSamples);
    
}
