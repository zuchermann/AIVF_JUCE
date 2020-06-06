/*
  ==============================================================================

    filterSynth.cpp
    Created: 28 May 2020 7:24:52pm
    Author:  YN

  ==============================================================================
*/

#include "FilterSynth1.h"

FilterSynth1::FilterSynth1() {
    filter_freqs = {1500.0f, 2000.0f, 300.0f, 3000.0f, 1000.0f, 200.0f, 500.0f, 1000.0f};
}

FilterSynth1::~FilterSynth1() {
}

void FilterSynth1::releaseResources() {
}

void FilterSynth1::event(int i){
    std::cout << i << "\n";
    int i_offset = (i + t_offset) % ((int) filter_freqs.size());
    float freq = filter_freqs[i_offset];
    filter_0->setCoefficients(IIRCoefficients::makeLowPass(sr, freq, Q));
    filter_1->setCoefficients(IIRCoefficients::makeLowPass(sr, freq, Q));
}

void FilterSynth1::setFrequency (float frequency)
{
    auto cyclesPerSample = frequency / sr;
    angleDelta = cyclesPerSample;
}

void FilterSynth1::setLevel (float lev) {
    level = lev;
}

AudioSourcePlayer* FilterSynth1::getSource() {
    audioSource.setSource(this);
    return &audioSource;
}

forcedinline float FilterSynth1::getNextSample() noexcept
{
    float noise = random.nextFloat() * 0.5f;
    auto currentSample = currentAngle + noise;
    updateAngle();
    return currentSample;
}

void FilterSynth1::prepareToPlay (int, double sampleRate) {
    sr = sampleRate;
    filter_0 = new IIRFilter();
    filter_1 = new IIRFilter();
    int num_freqs = (int) filter_freqs.size();
    int offset_i = ((num_freqs - 1) + t_offset) % num_freqs;
    filter_0->setCoefficients(IIRCoefficients::makeLowPass (sr, filter_freqs[offset_i], Q));
    filter_1->setCoefficients(IIRCoefficients::makeLowPass (sr, filter_freqs[offset_i], Q));
}

forcedinline void FilterSynth1::updateAngle() noexcept
{
    currentAngle += angleDelta;
    if (currentAngle >= 1.0)
        currentAngle -= 1.0;
}

void FilterSynth1::set_ts (std::vector<int> ts){
    int num_total = (int) filter_freqs.size();
    t_offset = ts[0] % num_total;
    float freq_offset = 30.0f * (ts[2] % 10);
    setFrequency(100 + freq_offset);
    float Q_offset = 1.0 * (ts[4] % 9);
    Q = 1.0 + Q_offset;
    int num_freqs = (int) filter_freqs.size();
    event(num_freqs - 1);
}

void FilterSynth1::start(){
    setLevel(0.5f);
}

void FilterSynth1::stop(){
    setLevel(0.0f);
}

void FilterSynth1::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
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
