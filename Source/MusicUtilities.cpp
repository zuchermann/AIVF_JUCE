/*
  ==============================================================================

    MusicUtilities.cpp
    Created: 12 Jun 2020 8:53:51pm
    Author:  YN

  ==============================================================================
*/

#include "MusicUtilities.h"

const int MusicUtilities::major_degree_to_pitchclass(int scale_degree)
{
    std::vector<int> major_scale = {0, 2, 4, 5, 7, 9, 11, 12};
    int octave = scale_degree / major_scale.size();
    return (12 * octave) + major_scale[scale_degree % major_scale.size()];
}

