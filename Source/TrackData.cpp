/*
  ==============================================================================

    TrackData.cpp
    Created: 13 Mar 2021 8:54:36am
    Author:  sapientex

  ==============================================================================
*/

#include "TrackData.h"
#include <filesystem>

TrackData::TrackData(File _file) : file(_file), 
                                   title(_file.getFileNameWithoutExtension())                                
{
    DBG("NICE JOB!!!" << title);
}

// bool TrackData::operator==(const juce::String& other) const 
// {
//     return title == other;
// }