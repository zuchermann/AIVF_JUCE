/*
  ==============================================================================

    SonificationScheduler.h
    Created: 11 May 2020 9:34:13pm
    Author:  YN

  ==============================================================================
*/

#pragma once

#include "Perc0Sampler.h"
#include "CSVLoader.h"
#include <JuceHeader.h>

class SonificationScheduler : public Timer
{
public:
    SonificationScheduler();
    void setVideoSystem(std::unique_ptr<VideoComponent> *videoSystem);
    void startSonification();
    void stopSonification();
    StringArray& getAllNames();
    void setId(String embryo_id);
private:
    void timerCallback () override;
    CSVLoader loader;
    std::unique_ptr<VideoComponent> *videoSystem;
    Perc0Sampler sampler;
    String file_name;
    int frame;
    double length;
    std::vector<int> t_events;
};
