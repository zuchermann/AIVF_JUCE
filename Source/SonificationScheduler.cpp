/*
  ==============================================================================

    SonificationScheduler.cpp
    Created: 11 May 2020 9:34:14pm
    Author:  YN

  ==============================================================================
*/

#include "Perc0Sampler.h"
#include "FilterSynth1.h"
#include "CSVLoader.h"
#include "SonificationScheduler.h"
#include "FileUtilities.h"

SonificationScheduler::SonificationScheduler(){
    audioDevice.initialise(0, 2, nullptr, true);
    
    //add all the sources
    audioDevice.addAudioCallback (sampler.getSource());
    audioDevice.addAudioCallback (filterSynth.getSource());
    audioDevice.addAudioCallback (melody1.getSource());
    audioDevice.addAudioCallback (melody2.getSource());
    
    frame = 0;
}

void SonificationScheduler::setVideoSystem(std::unique_ptr<VideoComponent> *videoSys){
    videoSystem = videoSys;
}

void SonificationScheduler::timerCallback() {
    //run t event based synthes
    for(int i = 0; i < t_events.size(); i ++){
        int this_event = t_events[i];
        if (this_event == frame){
            sampler.playSample(i);
            filterSynth.event(i);
            melody1.event(i);
        }
    }
    
    //run tick event based synths
    melody2.tick(frame);
    
    //run video
    double video_position = frame / 1.0;
    (*videoSystem)->setPlayPosition(video_position);
    //(*videoSystem)->play();
    //std::cout << video_position << "\n";
    
    //update frame
    frame++;
    if(frame > length){
        frame = 0;
    }
    //std::cout << loader.getNames().getReference(1) << "n";
}

void SonificationScheduler::setId(String embryo_id){
    //std::cout << embryo_id << "\n";
    stopSonification();
    file_name = embryo_id;
    URL vid_file = URL(FileUtilities::getFromResources(embryo_id));
    auto video_loaded = [this](auto& url, auto result) {
        length = (*videoSystem)->getVideoDuration();
        t_events = loader.getTs(file_name);
        
        //init synths here
        filterSynth.set_ts(t_events);
        melody1.set_ts(t_events);
        melody2.set_ts(t_events, length);
        setSuccess(loader.getSuccess(file_name));
    };
    
    (*videoSystem)->closeVideo();
    (*videoSystem)->loadAsync(vid_file, video_loaded);
}

StringArray& SonificationScheduler::getAllNames(){
    return loader.getNames();
}

void SonificationScheduler::setSuccess(float succ){
    //std::cout << succ << "\n";
    float inverse_succ = ((succ - 1.0) * -1.0);
    filterSynth.setHarmDist(inverse_succ * .15f);
    filterSynth.setNoise(0.5 + (succ * 0.5));
    filterSynth.setClip((inverse_succ * 50.0) + 1.0);
    
    melody1.setSuccess(succ);
    melody2.setSuccess(succ);
}

void SonificationScheduler::startSonification(){
    frame = 0;
    (*videoSystem)->setPlaySpeed(0);
    std::cout << file_name << "\n";
    //(*videoSystem)->play();
    startTimerHz(60);
    //filterSynth.setFrequency(440);
    
    filterSynth.start();
}

void SonificationScheduler::stopSonification(){
    frame = 0;
    stopTimer();
    filterSynth.stop();
}
