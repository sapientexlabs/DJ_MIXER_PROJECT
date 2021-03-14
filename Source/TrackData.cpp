/*
  ==============================================================================

    TrackData.cpp
    Created: 13 Mar 2021 8:54:36am
    Author:  sapientex

  ==============================================================================
*/

#include "TrackData.h"



TrackData::TrackData(juce::File _file) : file(_file), 
                                 title(_file.getFileNameWithoutExtension()),
                                 URL(juce::URL{ _file })
{
   std::cout << "Success TrackData::TrackData added"; 
}

bool TrackData::operator==(const juce::String& other) const 
{
    return title == other;
}