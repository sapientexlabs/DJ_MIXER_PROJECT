
#pragma once

#include <JuceHeader.h>
#include "DeckGUI.h"
#include "DJAudioPlayer.h"
#include "TrackData.h"
#include <vector> 
#include <string>
#include <fstream>
#include <algorithm>
#include <memory>


//==============================================================================
/*
*/
class PlaylistComponent  :public juce::Component,
                          public TableListBoxModel,
                          public Button::Listener
{
public:
    PlaylistComponent();
    
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    /*TableListBoxModel Functions*/
    int getNumRows() override;
    void paintRowBackground (Graphics &,
                                        int rowNumber, 
                                        int width, 
                                        int height,  
                                        bool rowIsSelected) override;
    void paintCell (Graphics &, 
                              int rowNumber, 
                              int columnId, int width, 
                              int height, bool rowIsSelected) override;

    Component * 	refreshComponentForCell (int rowNumber,
                                                          int columnId, 
                                                          bool isRowSelected, 
                                                          Component *existingComponentToUpdate);

   void buttonClicked(Button* button) override; 

   std::vector<TrackData> tracks;

private:

    TextButton importTracks{"+PLAYLIST"};
    TextButton addTrackDeckOne{"+DECKONE"};
    TextButton addTrackDeckTwo{"+DECKTWO"};
    TableListBox tableComponent;
    std::vector<std::string> trackTitles;
    std::vector<std::string> trackLengths;

    
    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2; 
    
    DJAudioPlayer* playerForParsingMetaData;

    void addTrackPlaylist();
    void saveTrackPlaylist();
    void loadTrackPlaylist();
    void deleteTrack(int id);
    
    
    
    bool checkDuplicateTrack(juce::String fileNameWithoutExtension);
    String getLength(juce::URL audioURL);
    String secondsToMinutes(double seconds); 
    

    void loadDeck(DeckGUI* deckGUI);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)

};

