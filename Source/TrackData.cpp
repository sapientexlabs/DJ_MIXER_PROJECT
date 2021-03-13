/*
  ==============================================================================

    TrackData.cpp
    Created: 13 Mar 2021 8:54:36am
    Author:  sapientex

  ==============================================================================
*/

#include "TrackData.h"
#include <filesystem>


TrackData::TrackData(juce::File _file) : file(_file), 
                                 title(_file.getFileNameWithoutExtension()),
                                 URL(juce::URL{ _file })
{
    DBG("Created new track with title: " << title);
}

bool TrackData::operator==(const juce::String& other) const 
{
    return title == other;
}