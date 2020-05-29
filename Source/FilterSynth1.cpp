/*
  ==============================================================================

    filterSynth.cpp
    Created: 28 May 2020 7:24:52pm
    Author:  YN

  ==============================================================================
*/

#include "FilterSynth1.h"

void FilterSynth1::setFrequency (float frequency)
{
    auto cyclesPerSample = frequency / sr;
    angleDelta = cyclesPerSample * MathConstants<float>::twoPi;
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
    auto currentSample = std::sin (currentAngle);
    updateAngle();
    return currentSample;
}

void FilterSynth1::prepareToPlay (int, double sampleRate) {
    sr = sampleRate;
}

forcedinline void FilterSynth1::updateAngle() noexcept
{
    currentAngle += angleDelta;
    if (currentAngle >= MathConstants<float>::twoPi)
        currentAngle -= MathConstants<float>::twoPi;
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
}
