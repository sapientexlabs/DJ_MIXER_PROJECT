#pragma once

#include <JuceHeader.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include "TrackData.h"
#include "DeckGUI.h"
#include "DJAudioPlayer.h"

//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
                           public juce::TableListBoxModel,
                           public juce::Button::Listener,
                           public juce::TextEditor::Listener
{
public:
    PlaylistComponent(DeckGUI* _deckGUI1, 
                      DeckGUI* _deckGUI2, 
                      DJAudioPlayer* _playerForParsingMetaData
                     );
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    int getNumRows() override;
    void paintRowBackground(juce::Graphics& g, 
                            int rowNumber, 
                            int width,
                            int height,
                            bool rowIsSelected
                           ) override;
    void paintCell(juce::Graphics& g,
                   int rowNumber,
                   int columnId,
                   int width,
                   int height,
                   bool rowIsSelected
                  ) override;
    
    Component* refreshComponentForCell(int rowNumber, 
                                       int columnId, 
                                       bool isRowSelected, 
                                       Component* existingComponentToUpdate) override;
    void buttonClicked(juce::Button* button) override;
    std::vector<TrackData> tracksData;
    TextButton importTracks{"+PLAYLIST"};
    TextButton addTrackDeckOne{"+DECKONE"};
    TextButton addTrackDeckTwo{"+DECKTWO"};

private:
  
    juce::TextEditor search;
    juce::TableListBox playlist;
    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;
    DJAudioPlayer* playerForParsingMetaData;
    
    juce::String getLength(juce::URL audioURL);
    juce::String getMinutes(double seconds);

    void addTrackPlaylist();

    void saveTrackPlaylist();
    void loadTrackPlaylist();
    void deleteTrack(int id);
    bool checkDuplicateTrack(juce::String fileNameWithoutExtension);
    int searchPlaylist(juce::String searchText);
    void addToPlayer(DeckGUI* deckGUI);
    

    void playlistSearch(juce::String searchText);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};