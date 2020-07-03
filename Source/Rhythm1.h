/*
  ==============================================================================

    Rhythm1.h
    Created: 2 Jul 2020 1:42:42pm
    Author:  YN

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Rhythm1
{
public:
    Rhythm1();
    ~Rhythm1();
    AudioSourcePlayer* getSource();
    void set_ts(std::vector<int> ts, double len);
    void tick(int i);
    void start();
    void stop();
private:
    const std::vector<String> filenames = {"DARBUKA.wav"};
    AudioFormatManager formatManager;
    AudioSourcePlayer audioSourcePlayer;
    AudioTransportSource transportSource;
    ResamplingAudioSource* resamplingSource;
    bool loadURLIntoTransport (const File& audioURL);
    bool isPlaying = false;
    std::vector<int> t_events;
    double video_length;
    std::unique_ptr<AudioFormatReaderSource> currentAudioFileSource;
    std::vector<File> rhythmFiles;
};
