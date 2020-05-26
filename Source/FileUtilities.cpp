/*
  ==============================================================================

    FileUtilities.cpp
    Created: 11 May 2020 2:04:21pm
    Author:  YN

  ==============================================================================
*/

#include "FileUtilities.h"

const File FileUtilities::getFromResources(String file_name)
{
    return ((File::getSpecialLocation (File::currentExecutableFile)).getParentDirectory()).getChildFile(file_name);
}
