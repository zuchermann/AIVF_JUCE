/*
  ==============================================================================

    Perc0Sampler.cpp
    Created: 9 May 2020 4:41:48pm
    Author:  YN

  ==============================================================================
*/

#include "Perc0Sampler.h"
#include "FileUtilities.h"


Perc0Sampler::Perc0Sampler()
{
    //audioDeviceManager.initialise(2, 2, nullptr, true);
    //audioDeviceManager.initialiseWithDefaultDevices(2, 2);
    formatManager.registerBasicFormats();
    
    //AudioDeviceManager();
    
    audioSourcePlayer.setSource (&transportSource);
    
    //gamalan1 = File::getSpecialLocation (File::currentExecutableFile);
    //gamalan1 = gamalan1.getParentDirectory();
    //gamalan1 = gamalan1.getChildFile("shake_bip.aif");
    
}

Perc0Sampler::~Perc0Sampler()
{
    transportSource  .setSource (nullptr);
    audioSourcePlayer.setSource (nullptr);
}

AudioSourcePlayer* Perc0Sampler::getSource(){
    return &audioSourcePlayer;
}

const std::vector<String> Perc0Sampler::FILE_NAMES {"bass1_bip.aif", "bass2_bip.aif", "hand_drum1_bip.aif", "hand_drum2_bip.aif", "hand_drum3_bip.aif", "hand_drum4_bip.aif", "shake_bip.aif", "triangle_bip.aif"};

bool Perc0Sampler::loadURLIntoTransport (const File& audioFile)
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

void Perc0Sampler::playSample(int samp_idx)
{
    //std::cout << audioSourcePlayer.getGain() << "\n";
    //audioDeviceManager.playTestSound();
    //std::cout << String(transportSource.getLengthInSeconds()) << "\n";
    loadURLIntoTransport(FileUtilities::getFromResources(FILE_NAMES[samp_idx]));
    transportSource.setPosition(0);
    transportSource.start();
}
