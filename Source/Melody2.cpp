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
    succ = adjusted_succ; //fail represented by 1, success by 0
    generateMelody();
    melody_events = std::vector<int>(lengths.size());
    
    //turn representation of rhythm in lengths vector into frame numbers
    int num_events = (int) lengths.size();
    //std::cout << num_events << " rhyth \n";
    
    double current_sum = 0.0;
    for(int i = 0; i < num_events; i++){
       int frame_i = (int) ((current_sum/beats_per_vid) * vid_length);
       melody_events[i] = frame_i;
       //std::cout << frame_i << "\n";
       current_sum += lengths[i];
    }
}

//generates unique melody for each emryo based on t events and success value.
void Melody2::generateMelody() {
    std::vector<float> strong_notes = {0,0,0,0,4,4,2,7};
    std::vector<float> other_notes = {1,2,3,5,5,5,6,6,7};
    std::vector<double> strong_times = std::vector<double>(beats_per_vid / 2);
    for(int i = 0; i < beats_per_vid / 2; i++){
        strong_times[i] = (double) i*2;
    }
    std::vector<float> used_notes = {};
    std::vector<float> all_notes = {0,1,2,3,4,5,6,7};
    
    double currentLen = 0.;
    int note_count = 0;
    lengths = {};
    pitches = {};
    
    while(currentLen < beats_per_vid){
        int t_2 = t_events[(note_count) % t_events.size()];
        int t_3 = t_events[(note_count + 1) % t_events.size()];
        int t_4 = t_events[(note_count + 2) % t_events.size()];
        int t_5 = t_events[(note_count + 3) % t_events.size()];
        int t_6 = t_events[(note_count + 4) % t_events.size()];
        int t_7 = t_events[(note_count + 5) % t_events.size()];
        int t_8 = t_events[(note_count + 6) % t_events.size()];
        int t_9 = t_events[(note_count + 7) % t_events.size()];
        
        //GENERATE NEXT NOTE...
        
        
        //default note
        int new_note = other_notes[(t_2 + t_3 + t_8) % other_notes.size()];
        
        //if strong beat for importantnote
        if(std::find(strong_times.begin(), strong_times.end(), currentLen) != strong_times.end()){
            //std::cout << "strong beat" << "\n";
            float rand_0 = ((float) (t_events[t_4 + t_5 + t_9] % 10)) / 10.;
            if(succ <= rand_0){
                //std::cout << "doing a strong note fr" << "\n";
                new_note = strong_notes[t_events[t_6 + t_7] % strong_notes.size()];
            }
        }
        
        //if first time note is used
        if(!(std::find(used_notes.begin(), used_notes.end(), new_note) != used_notes.end())){
            used_notes.push_back(new_note);
            std::cout << "note " << new_note << " used. " << used_notes.size() << " notes used." <<"\n";
        }
        
        pitches.push_back(new_note);
        
        //*************************************************
        //GENERATE NEXT NOTE RHYTHM
        
        int quant_succ = (int)(succ * 5.);
        double hop_size = pow(0.5, ((double)(2 + quant_succ)));
        double mult = (1./hop_size);
        double rhythm = hop_size + (((t_2+t_9+t_8*t_9) % (int) mult) * hop_size);
        
        if ((currentLen != floor(currentLen)) and (floor(currentLen + rhythm) > floor(currentLen))){
            if ((t_2 % 10) >= (succ * 10)){
                rhythm = floor(currentLen + rhythm) - currentLen;
            }
        }
            
        double next_rhythm = rhythm;
        
        //std::cout << pitches.size() << " rhyth \n";
        
        lengths.push_back(next_rhythm);
        currentLen += next_rhythm;
        note_count += 1;
        
    }
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
    t_events = ts;
    vid_length = len;
    base_pitch = 72 + ts[2] % 12;
    next_to_play = 0;
    //std::cout << vid_length << "LENGTH \n";
    
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
