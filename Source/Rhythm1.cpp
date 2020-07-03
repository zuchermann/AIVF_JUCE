/*
  ==============================================================================

    Rhythm1.cpp
    Created: 2 Jul 2020 1:42:42pm
    Author:  YN

  ==============================================================================
*/

#include "FileUtilities.h"
#include "Rhythm1.h"

Rhythm1::Rhythm1() {
    formatManager.registerBasicFormats();
    resamplingSource = new ResamplingAudioSource(&transportSource, true);
    audioSourcePlayer.setSource (resamplingSource);
}

Rhythm1::~Rhythm1() {
    transportSource  .setSource (nullptr);
    audioSourcePlayer.setSource (nullptr);
    delete resamplingSource;
}

AudioSourcePlayer* Rhythm1::getSource(){
    return &audioSourcePlayer;
}

bool Rhythm1::loadURLIntoTransport (const File& audioFile)
{
    // unload the previous file source and delete it..
    transportSource.stop();
    transportSource.setSource (nullptr);
    currentAudioFileSource.reset();

    AudioFormatReader* reader = nullptr;
    
    #if ! JUCE_IOS
    if (audioURL.isLocalFile())
    {
        reader = formatManager.createReaderFor (audioFile.getLocalFile());
    }
    else
    #endif
    {
        if (reader == nullptr)
            reader = formatManager.createReaderFor (audioFile);
    }

    if (reader != nullptr)
    {
        currentAudioFileSource.reset (new AudioFormatReaderSource (reader, true));

        // ..and plug it into our transport source
        transportSource.setSource(currentAudioFileSource.get());
        //transportSource.setSource ( currentAudioFileSource.get(), 32768, nullptr, reader->sampleRate);
        return true;
    }
    return false;
}

void Rhythm1::tick(int i){
    if (isPlaying and i == 0){
        File audio_file = FileUtilities::getFromResources(filenames[0]);
        loadURLIntoTransport(audio_file);
        transportSource.setPosition(0);
        double audio_len = transportSource.getLengthInSeconds();
        //std::cout << audio_len / video_length << "\n";
        resamplingSource->setResamplingRatio(audio_len / video_length);
        
        transportSource.start();
    }
}

void Rhythm1::set_ts(std::vector<int> ts, double len){
    video_length = len;
    t_events = ts;
    std::cout << video_length << "\n";
}

void Rhythm1::start(){
    //std::cout << audioSourcePlayer.getGain() << "\n";
    //audioDeviceManager.playTestSound();
    isPlaying = true;
}

void Rhythm1::stop(){
    transportSource.stop();
    isPlaying = false;
}
