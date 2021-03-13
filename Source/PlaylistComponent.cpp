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
    search.onReturnKey = [this] { searchLibrary (search.getText()); };

    playlist.setModel(this); 
    playlist.getHeader().addColumn("Track Title", 1, 200);
    playlist.getHeader().addColumn("Length of Track", 2, 200);;
    playlist.getHeader().addColumn("", 3, 1);
    loadLibrary();
}

PlaylistComponent::~PlaylistComponent()
{
    saveLibrary();
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::white);
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
    return tracks.size();
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
            g.drawText(tracks[rowNumber].title,
                                                2, 
                                                0,
                                                width-4, 
                                                height, 
                                                Justification::centredLeft,
                                                true);
        }
        if (columnId == 2)
        {
            g.drawText(tracks[rowNumber].length,
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
        importToLibrary();
        playlist.updateContent();
    }
    else if (button == &addTrackDeckOne)
    {
        loadInPlayer(deckGUI1);
    }
    else if (button == &addTrackDeckTwo)
    {
        loadInPlayer(deckGUI2);
    }
    else
    {
        int id = std::stoi(button->getComponentID().toStdString());
        deleteFromTracks(id);
        playlist.updateContent();
    }
}

void PlaylistComponent::loadInPlayer(DeckGUI* deckGUI)
{
    int selectedRow{playlist.getSelectedRow()};
    if (selectedRow != -1)
    {
        
        deckGUI->loadAudio(tracks[selectedRow].URL);
    }
    else
    {
       std::cout << "NOT FOUND" << std::endl; 
    }
}

void PlaylistComponent::importToLibrary()
{
    FileChooser chooser{"Audio File..."};
    if (chooser.browseForMultipleFilesToOpen())
    {
        for (const juce::File& file : chooser.getResults())
        {
            juce::String fileNameWithoutExtension{file.getFileNameWithoutExtension()};
            if (!isInTracks(fileNameWithoutExtension))
            {
                TrackData newTrackData{ file };
                juce::URL audioURL{ file };
                newTrackData.length = getLength(audioURL) ;
                tracks.push_back(newTrackData);
            }
          }
        }
}


bool PlaylistComponent::isInTracks(juce::String fileNameWithoutExtension)
{
    return (std::find(tracks.begin(), tracks.end(), fileNameWithoutExtension) != tracks.end());
}

void PlaylistComponent::deleteFromTracks(int id)
{
    tracks.erase(tracks.begin() + id);
}

juce::String PlaylistComponent::getLength(juce::URL audioURL)
{
    playerForParsingMetaData->loadURL(audioURL);
    double seconds{ playerForParsingMetaData->getSeconds() };
    juce::String minutes{ secondsToMinutes(seconds) };
    return minutes;
}

juce::String PlaylistComponent::secondsToMinutes(double seconds)
{
    int secondsRounded{ int(std::round(seconds)) };
    juce::String min{ std::to_string(secondsRounded / 60) };
    juce::String sec{ std::to_string(secondsRounded % 60) };
    
    if (sec.length() < 2) // if seconds is 1 digit or less
    {
        sec = sec.paddedLeft('0', 2);
    }
    return juce::String{ min + ":" + sec };
}

void PlaylistComponent::searchLibrary(juce::String searchText)
{
    if (searchText != "")
    {
        int rowNumber = whereInTracks(searchText);
        playlist.selectRow(rowNumber);
    }
    else
    {
        playlist.deselectAllRows();
    }
}

int PlaylistComponent::whereInTracks(juce::String searchText)
{
    auto it = find_if(tracks.begin(), tracks.end(), 
        [&searchText](const TrackData& obj) {return obj.title.contains(searchText); });
    int i = -1;

    if (it != tracks.end())
    {
        i = std::distance(tracks.begin(), it);
    }

    return i;
}

void PlaylistComponent::saveLibrary()
{
    std::ofstream playlist("playlist.csv");
    for (TrackData& a : tracks)
    {
        playlist << a.file.getFullPathName() << "," << a.length << "\n";
    }
}

void PlaylistComponent::loadLibrary()
{
    std::ifstream playlist("playlist.csv");
    std::string filePath;
    std::string length;
    
    if (playlist.is_open())
    {
        while (getline(playlist, filePath, ',')) {
            juce::File file{file};
            TrackData newTrackData{ file };

            getline(playlist, length);
            newTrackData.length = length;
            tracks.push_back(newTrackData);
        }
    }
    playlist.close();
}