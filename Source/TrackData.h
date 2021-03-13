/*
  ==============================================================================

    TrackData.h
    Created: 13 Mar 2021 8:54:36am
    Author:  sapientex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class TrackData
{
    public:
        TrackData(juce::File _file);
        File file;
        String title;
        String length;       
};

//juce::URL URL;
        /**objects are compared by title*/
        //bool operator==(const juce::String& other) const;//