/*
  ==============================================================================

    TrackData.h
    Created: 13 Mar 2021 8:54:36am
    Author:  sapientex

  ==============================================================================
*/
#pragma once
#include <filesystem>
#include <JuceHeader.h>

class TrackData
{
    public:
        TrackData(juce::File _file);
        juce::File file;
        juce::URL URL;
        juce::String title;
        juce::String length;
        /**objects are compared by title*/
        bool operator==(const juce::String& other) const;
}; 