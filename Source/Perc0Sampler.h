/*
  ==============================================================================

    Perc0Sampler.h
    Created: 9 May 2020 4:41:48pm
    Author:  YN

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Perc0Sampler
{
public:
    Perc0Sampler();
    ~Perc0Sampler();
    void playSample(int sample_index);
    
    const static int KICK1 = 0;
    const static int KICK2 = 1;
    const static int DRUM1 = 2;
    const static int DRUM2 = 3;
    const static int DRUM3 = 4;
    const static int DRUM4 = 5;
    const static int SHAKE = 6;
    const static int TRIANGLE = 7;
    const static std::vector<String> FILE_NAMES;

private:
    AudioDeviceManager audioDeviceManager;
    AudioFormatManager formatManager;
    AudioSourcePlayer audioSourcePlayer;
    AudioTransportSource transportSource;
    bool loadURLIntoTransport (const File& audioURL);
    std::unique_ptr<AudioFormatReaderSource> currentAudioFileSource;
    std::vector<File> percFiles;
};
