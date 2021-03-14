/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 13 Mar 2021 8:34:20am
    Author:  sapientex

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(DeckGUI* _deckGUI1, 
                                     DeckGUI* _deckGUI2,
                                     DJAudioPlayer* _playerForParsingMetaData
                                    ) : deckGUI1(_deckGUI1),
                                        deckGUI2(_deckGUI2),
                                        playerForParsingMetaData(_playerForParsingMetaData)
{   
    addAndMakeVisible(playlist);
    addAndMakeVisible(importTracks);
    addAndMakeVisible(addTrackDeckOne);
    addAndMakeVisible(addTrackDeckTwo);
    addAndMakeVisible(search);
    
    
    importTracks.addListener(this);
    addTrackDeckTwo.addListener(this);
    addTrackDeckOne.addListener(this);
    search.addListener(this);
   
    search.setTextToShowWhenEmpty("Search", 
                                  juce::Colours::red);
    search.onReturnKey = [this] { playlistSearch(search.getText()); };

    playlist.setModel(this); 
    playlist.getHeader().addColumn("Track Title", 1, 200);
    playlist.getHeader().addColumn("Length of Track", 2, 200);;
    playlist.getHeader().addColumn("", 3, 1);
    loadTrackPlaylist();
}

PlaylistComponent::~PlaylistComponent()
{
    saveTrackPlaylist();
}

void PlaylistComponent::paint (juce::Graphics& g)
{
   //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
    g.fillAll(Colours::black); 
    g.setColour (juce::Colours::red);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::red);
    g.setFont (14.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    double rowA = getHeight() / 14;
    double columnA = getWidth() / 16;
    
    importTracks.setBounds(columnA - columnA,rowA*11, getWidth()/16, playlist.getHeader().getHeight());
    addTrackDeckOne.setBounds(columnA*2,rowA*11, getWidth()/16, playlist.getHeader().getHeight());
    addTrackDeckTwo.setBounds(columnA*3,rowA*11, getWidth()/16, playlist.getHeader().getHeight());
    search.setBounds(0, 14 * getHeight() / 16, getWidth(), getHeight() / 16);
    
    playlist.setBounds(0, rowA, getWidth(), columnA *14); 
}
int PlaylistComponent::getNumRows()
{
    return tracksData.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g,
                                           int rowNumber,
                                           int width,
                                           int height,
                                           bool rowIsSelected
                                          )
{
    if (rowIsSelected)
    {
        g.fillAll(juce::Colours::red);
    }
    else
    {
        g.fillAll(juce::Colours::darkgrey);
    }
}

void PlaylistComponent::paintCell(juce::Graphics& g,
                                  int rowNumber,
                                  int columnId,
                                  int width,
                                  int height,
                                  bool rowIsSelected
                                 )
{
    if (rowNumber < getNumRows())
    {
        if (columnId == 1)
        {
            g.drawText(tracksData[rowNumber].title,
                                                2, 
                                                0,
                                                width-4, 
                                                height, 
                                                Justification::centredLeft,
                                                true);
        }
        if (columnId == 2)
        {
            g.drawText(tracksData[rowNumber].length,
                                                2, 
                                                0,
                                                width-4, 
                                                height, 
                                                Justification::centredLeft,
                                                true);
            }
    }
}

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
                                                      int columnId,
                                                      bool isRowSelected,
                                                      Component* existingComponentToUpdate)
{
    if (columnId == 3)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton{"DELETE"};
            String id{std::to_string(rowNumber)};
            btn->setComponentID(id);
            btn->addListener(this); 
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
    if (button == &importTracks)
    {
        addTrackPlaylist();
        playlist.updateContent();
    }
    else if (button == &addTrackDeckOne)
    {
       addToPlayer(deckGUI1);
    }
    else if (button == &addTrackDeckTwo)
    {
        addToPlayer(deckGUI2);
    }
    else
    {
        int id = std::stoi(button->getComponentID().toStdString());
        deleteTrack(id);
        playlist.updateContent();
    }
}

void PlaylistComponent::addToPlayer(DeckGUI* deckGUI)
{
    int selectedRow{playlist.getSelectedRow()};
    if (selectedRow != -1)
    {
        
        deckGUI->loadAudio(tracksData[selectedRow].URL);
    }
    else
    {
       std::cout << "NOT FOUND" << std::endl; 
    }
}

void PlaylistComponent::addTrackPlaylist()
{
    FileChooser chooser{"Audio File..."};
    if (chooser.browseForMultipleFilesToOpen())
    {
        for (const juce::File& file : chooser.getResults())
        {
            juce::String fileNameWithoutExtension{file.getFileNameWithoutExtension()};
            if (!checkDuplicateTrack(fileNameWithoutExtension))
            {
                TrackData newTrackData{ file };
                juce::URL audioURL{ file };
                newTrackData.length = getLength(audioURL) ;
                tracksData.push_back(newTrackData);
            }
          }
        }
}


bool PlaylistComponent::checkDuplicateTrack(juce::String fileNameWithoutExtension)
{
    return (std::find(tracksData.begin(), tracksData.end(), fileNameWithoutExtension) != tracksData.end());
}

void PlaylistComponent::deleteTrack(int id)
{
    tracksData.erase(tracksData.begin() + id);
}

juce::String PlaylistComponent::getLength(juce::URL audioURL)
{
    playerForParsingMetaData->loadURL(audioURL);
    double sec{ playerForParsingMetaData->getSeconds() };
    juce::String min{ getMinutes(sec) };
    return min;
}

juce::String PlaylistComponent::getMinutes(double seconds)
{
    int Rounded{int(std::round(seconds))};
    juce::String m{std::to_string(Rounded / 60)};
    juce::String s{std::to_string(Rounded % 60)};
    
    if (s.length() < 2) // if seconds is 1 digit or less
    {
        s = s.paddedLeft('0', 2);
    }
    return String{ m + ":" + s };
}

void PlaylistComponent::playlistSearch(juce::String searchText)
{
    if (searchText != "")
    {
        int rowNumber = searchPlaylist(searchText);
        playlist.selectRow(rowNumber);
    }
    else
    {
        playlist.deselectAllRows();
    }
}

int PlaylistComponent::searchPlaylist(juce::String searchText)
{
    auto it = find_if(tracksData.begin(), tracksData.end(), 
        [&searchText](const TrackData& obj) {return obj.title.contains(searchText); });
    int i = -1;

    if (it != tracksData.end())
    {
        i = std::distance(tracksData.begin(), it);
    }

    return i;
}

void PlaylistComponent::saveTrackPlaylist()
{
    std::ofstream playlistCSV("playlist.csv");
    for (TrackData& t : tracksData)
    {
        playlistCSV << t.file.getFullPathName() << "," << t.length << "\n";
    }
}

void PlaylistComponent::loadTrackPlaylist()
{
    std::ifstream playlistCSV("playlist.csv");
    std::string filePath;
    std::string length;
    
    if (playlistCSV.is_open())
    {
        while (getline(playlistCSV, filePath, ',')) {
            juce::File file{file};
            TrackData newTrackData{file};

            getline(playlistCSV, length);
            newTrackData.length = length;
            tracksData.push_back(newTrackData);
        }
    }
    playlistCSV.close();
}