/*
  ==============================================================================

    Melody2.cpp
    Created: 12 Jun 2020 8:02:01pm
    Author:  YN

  ==============================================================================
*/

#include "Melody2.h"
#include "MusicUtilities.h"

Melody2::Melody2() {
    //filter_freqs = {1500.0f, 2000.0f, 300.0f, 3000.0f, 1000.0f, 200.0f, 500.0f, 1000.0f};
    lengths = {
        .25,.25,.25,.25,
        .25,.25,.25,.25,
        .25,.25,.25,.25,
        .25,.25,.25,.25,
    }; //lengths of notes; 1 = 1 "beat". Each video is thought of as 1 4-beat bar
    pitches = {
        0,2,4,7,
        1,3,5,8,
        4,2,4,1,
        4,3,2,1
    }; //pitches are scale degrees of major scale
    
    rhythm_error = {
        0.1, 0, 0.7, 0.3, 0.9, 0.2, 0.5
    };//specifies how rhythm will be screwed with according to success values
    
    melody_events = std::vector<int>(lengths.size());
}

Melody2::~Melody2() {
}

void Melody2::releaseResources() {
}

float Melody2::midiToFreq(float midi_val) {
    return  440.0 * (std::pow (2.0f, (midi_val - 69.0f) / 12.0f));
}

void Melody2::setSuccess(float success) {
    float adjusted_succ = (success - 1.0f) * -1.0f;
    succ = adjusted_succ;
}

void Melody2::tick(int i){
    //std::vector<int> pitch_classes = {0, 2, 4, 1, 6, 3, 5, 7};
    //int pitch_class = pitch_classes[i];
    //std::vector<float> detune_dirs = {1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f};
    //float detune_dir = detune_dirs[pitch_class] * 3.0f;
    for(int j = 0; j < lengths.size(); j++){
        int curr_event = melody_events[j];
        double curr_len = lengths[j];
        
        double mult = rhythm_error[j % rhythm_error.size()];
        int offset = (int) (((succ * curr_len * mult) / beats_per_vid) * vid_length);
        if(i == curr_event + offset){
            int scale_degree = (int) pitches[j];
            float detune_dir = detune_dirs[scale_degree] * 1.0f;
            int pitch_class = MusicUtilities::major_degree_to_pitchclass((int) pitches[j]);
            float this_pitch = (succ * detune_dir) + ((float) pitch_class) + ((float) base_pitch);
            //std::cout << this_pitch << "\n";
            float freq = midiToFreq(this_pitch);
            //std::cout << freq << "\n";
            setFrequency(freq);
        }
    }
    //std::cout << i << "\n";
    //int i_offset = (i + t_offset) % ((int) filter_freqs.size());
    //float freq = filter_freqs[i_offset];
    //filter_0->setCoefficients(IIRCoefficients::makeLowPass(sr, freq, Q));
    //filter_1->setCoefficients(IIRCoefficients::makeLowPass(sr, freq, Q));
}

void Melody2::setFrequency (float frequency)
{
    auto cyclesPerSample = frequency / sr;
    auto envPerSample = env_freq / sr;
    angleDelta = cyclesPerSample;
    currentAngle = 0.0f;
    env = 1.0;
    env_delta = envPerSample;
}

AudioSourcePlayer* Melody2::getSource() {
    audioSource.setSource(this);
    return &audioSource;
}

forcedinline float Melody2::getNextSample() noexcept
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

void Melody2::prepareToPlay (int, double sampleRate) {
    sr = sampleRate;
    //filter_0 = new IIRFilter();
    //filter_1 = new IIRFilter();
    //int num_freqs = (int) filter_freqs.size();
    //int offset_i = ((num_freqs - 1) + t_offset) % num_freqs;
    //filter_0->setCoefficients(IIRCoefficients::makeLowPass (sr, filter_freqs[offset_i], Q));
    //filter_1->setCoefficients(IIRCoefficients::makeLowPass (sr, filter_freqs[offset_i], Q));
}

forcedinline void Melody2::updateAngle() noexcept
{
    currentAngle += angleDelta;
    if (currentAngle >= twoPi)
        currentAngle -= twoPi;
}

forcedinline void Melody2::updateEnv() noexcept {
    env -= env_delta;
    if (env < 0.0f)
        env = 0.0f;
}

void Melody2::set_ts (std::vector<int> ts, double len){
    //int num_total = (int) filter_freqs.size();
    //t_offset = ts[0] % num_total;
    vid_length = len;
    base_pitch = 72 + ts[2] % 12;
    next_to_play = 0;
    //std::cout << vid_length << "LENGTH \n";
    
    //turn representation of rhythm in lengths vector into frame numbers
    int num_events = (int) lengths.size();
    double current_sum = 0.0;
    for(int i = 0; i < num_events; i++){
        int frame_i = (int) ((current_sum/beats_per_vid) * vid_length);
        melody_events[i] = frame_i;
        //std::cout << frame_i << "\n";
        current_sum += lengths[i];
    }
    
}

void Melody2::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
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
