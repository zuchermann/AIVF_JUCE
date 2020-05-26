/*
  ==============================================================================

    Class for loading in a csv file into a c++ map
 
    CSVLoader.h
    Created: 10 May 2020 8:33:12pm
    Author:  YN

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class CSVLoader
{
public:
    CSVLoader();
    StringArray& getNames();
    StringArray& getEntry(String& entryName);
    std::vector<int> getTs(String& entryName);
    const static int NAME_COLUMN_INDEX = 1;
    
private:
    std::map<juce::String, juce::StringArray> csvMap;
    StringArray data_labels;
    StringArray embyo_ids;
};
