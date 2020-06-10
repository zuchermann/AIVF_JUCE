/*
  ==============================================================================

    FilterSynth2.cpp
    Created: 5 Jun 2020 7:56:27pm
    Author:  YN

  ==============================================================================
*/

#include "FilterSynth2.h"

FilterSynth2::FilterSynth2() {
    currentAngles = std::vector<float>(num_sines);
    angleDeltas = std::vector<float>(num_sines);
    amps = std::vector<float>(num_sines);
    for(int i = 0; i < num_sines; i++){
        float part_num = (i + 1) * 1.0f;
        float amp = 1.0f / part_num;
        amps[i] = amp;
        currentAngles[i] = 0.0f;
        angleDeltas[i] = 0.0f;
        //std::cout << amps[i] << " ";
    }
    //std::cout << twoPi << "\n";
    filter_freqs = {1500.0f, 2000.0f, 300.0f, 3000.0f, 1000.0f, 200.0f, 500.0f, 1000.0f};
}

FilterSynth2::~FilterSynth2() {
}

void FilterSynth2::releaseResources() {
}

void FilterSynth2::event(int i){
    //std::cout << i << "\n";
    int i_offset = (i + t_offset) % ((int) filter_freqs.size());
    float freq = filter_freqs[i_offset];
    filter_0->setCoefficients(IIRCoefficients::makeLowPass(sr, freq, Q));
    filter_1->setCoefficients(IIRCoefficients::makeLowPass(sr, freq, Q));
}

void FilterSynth2::setFrequency (float frequency)
{
    for (int i = 0; i < num_sines; i++){
        float mult = (i + 1) * 1.0f;
        float dist = (i % 2 == 0) ?  harm_distortion * -1.0f : harm_distortion;
        auto cyclesPerSample = (frequency * (mult + dist)) / sr;
        angleDeltas[i] = cyclesPerSample;
    }
    freq = frequency;
}

void FilterSynth2::setNoise(float noise) {
    noise_lev = noise;
}

void FilterSynth2::setHarmDist(float dist) {
    harm_distortion = dist;
    setFrequency(freq);
}


void FilterSynth2::setLevel (float lev) {
    level = lev;
}

void FilterSynth2::setClip(float clip_amt) {
    clip = clip_amt;
}

AudioSourcePlayer* FilterSynth2::getSource() {
    audioSource.setSource(this);
    return &audioSource;
}

float clamp(float n, float lower, float upper) {
  return std::max(lower, std::min(n, upper));
}

forcedinline float FilterSynth2::getNextSample() noexcept
{
    float noise = random.nextFloat() * noise_lev;
    float currentSample = 0.0f;
    for(int i = 0; i < num_sines; i++){
        float comp_sin = (float) std::sin(currentAngles[i]);
        currentSample += (comp_sin * amps[i]);
    }
    currentSample = clamp(currentSample * clip, 0.0f, 1.0f);
    currentSample += noise;
    updateAngle();
    return currentSample;
}

void FilterSynth2::prepareToPlay (int, double sampleRate) {
    sr = sampleRate;
    filter_0 = new IIRFilter();
    filter_1 = new IIRFilter();
    int num_freqs = (int) filter_freqs.size();
    int offset_i = ((num_freqs - 1) + t_offset) % num_freqs;
    filter_0->setCoefficients(IIRCoefficients::makeLowPass (sr, filter_freqs[offset_i], Q));
    filter_1->setCoefficients(IIRCoefficients::makeLowPass (sr, filter_freqs[offset_i], Q));
}

forcedinline void FilterSynth2::updateAngle() noexcept
{
    for(int i = 0; i < num_sines; i++){
        currentAngles[i] += angleDeltas[i];
        if (currentAngles[i] >= twoPi)
            currentAngles[i] -= twoPi;
    }
}

void FilterSynth2::set_ts (std::vector<int> ts){
    int num_total = (int) filter_freqs.size();
    t_offset = ts[0] % num_total;
    int pitch_offset = ts[2] % 12;
    float freq = (float) MidiMessage::getMidiNoteInHertz(60 + pitch_offset);
    setFrequency(freq);
    float Q_offset = 1.0 * (ts[4] % 9);
    Q = 1.0 + Q_offset;
    int num_freqs = (int) filter_freqs.size();
    event(num_freqs - 1);
}

void FilterSynth2::start(){
    setLevel(0.5f);
}

void FilterSynth2::stop(){
    setLevel(0.0f);
}

void FilterSynth2::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    auto* leftBuffer  = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample); // [7]
    auto* rightBuffer = bufferToFill.buffer->getWritePointer (1, bufferToFill.startSample);
    
 
    bufferToFill.clearActiveBufferRegion();
    
    for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
    {
        auto levelSample = getNextSample() * level;       // [9]
        
        leftBuffer[sample]  += levelSample;                           // [10]
        rightBuffer[sample] += levelSample;
    }
    
    filter_0->processSamples(leftBuffer, bufferToFill.numSamples);
    filter_1->processSamples(rightBuffer, bufferToFill.numSamples);
}
