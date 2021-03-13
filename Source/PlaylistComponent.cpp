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
PlaylistComponent::PlaylistComponent( )
{
   trackTitles.push_back("Track 1"); 
   trackLengths.push_back("3:00"); 
   trackTitles.push_back("Track 2"); 
   trackLengths.push_back("3:00"); 
   trackTitles.push_back("Track 3"); 
   trackLengths.push_back("3:00"); 
   trackTitles.push_back("Track 4"); 
   trackLengths.push_back("3:00"); 
   trackTitles.push_back("Track 5"); 
   trackLengths.push_back("3:00"); 
   
   tableComponent.getHeader().addColumn("Track Title", 1, 200);
   tableComponent.getHeader().addColumn("Length of Track", 2, 200);
   tableComponent.getHeader().addColumn("Delete Track", 3, 100);


   tableComponent.setModel(this);

   addAndMakeVisible(tableComponent);
    
   addAndMakeVisible(importTracks);
   addAndMakeVisible(addTrackDeckOne);
   addAndMakeVisible(addTrackDeckTwo);

   importTracks.addListener(this);
   addTrackDeckOne.addListener(this);
   addTrackDeckTwo.addListener(this);
   
   loadTrackPlaylist();
}

PlaylistComponent::~PlaylistComponent()
{
  saveTrackPlaylist();
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
    tableComponent.setBounds(0, rowA, getWidth(), getHeight()); 
    importTracks.setBounds(columnA - columnA,rowA*12, getWidth()/16, tableComponent.getHeader().getHeight());
    addTrackDeckOne.setBounds(columnA*2,rowA*12, getWidth()/16, tableComponent.getHeader().getHeight());
    addTrackDeckTwo.setBounds(columnA*3,rowA*12, getWidth()/16, tableComponent.getHeader().getHeight());
}
int PlaylistComponent::getNumRows()
{
 return tracks.size();  
}
void PlaylistComponent::paintRowBackground (Graphics &g,
                             int rowNumber, 
                             int width, 
                             int height, 
                             bool rowIsSelected)
{
  if(rowIsSelected)
  {
   g.fillAll(Colours::red);
  }
  else{
   g.fillAll(Colours::darkgrey);
  }
}
void PlaylistComponent::paintCell (Graphics &g, 
                    int rowNumber, 
                    int columnId, int width, 
                    int height, bool rowIsSelected)
{
  if(columnId == 1){
  g.drawText(tracks[rowNumber].title,
                                    2, 
                                    0,
                                    width-4, 
                                    height, 
                                    Justification::centredLeft,
                                    true);
   }
  if(columnId == 2){
  g.drawText(tracks[rowNumber].length,
                                    2, 
                                    0,
                                    width-4, 
                                    height, 
                                    Justification::centredLeft,
                                    true);
  }
}
Component * PlaylistComponent::refreshComponentForCell (int rowNumber,
                                                          int columnId, 
                                                          bool isRowSelected, 
                                                          Component *existingComponentToUpdate)
{
  if(columnId == 3)
    {
      if(existingComponentToUpdate == nullptr)
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

   void PlaylistComponent::buttonClicked(Button* button)
   {
    

    if(button == &importTracks)
    {
        std::cout << "import button was clicked" << std::endl;
        addTrackPlaylist(); 
        tableComponent.updateContent();  
    }
    else if(button == &addTrackDeckOne){
      
      std::cout << "ad1 button was clicked" << std::endl;
    }
    else if(button == &addTrackDeckTwo){
      std::cout << "ad2 button was clicked" << std::endl;
    }
    else{
    int id = std::stoi(button->getComponentID().toStdString());
    std::cout << " PlaylistComponent::buttonClicked Button CLicked " << trackTitles[id] << std::endl;
    deleteTrack(id);
    tableComponent.updateContent(); 
    }
  }
 
   void PlaylistComponent::addTrackPlaylist()
  { 
   juce::FileChooser chooser{ "Select files" };
    if (chooser.browseForMultipleFilesToOpen())
    {
       for (const juce::File& file : chooser.getResults())
        {
      String fileNameWithoutExtension{file.getFileNameWithoutExtension()};
      String fileFullPathName{file.getFullPathName()};
      // if (!checkDuplicateTrack(fileNameWithoutExtension))
      // {
        TrackData newTrackData{file};
        //juce::URL URL{file};
        //newTrackData.length = getLength(URL) ;
        tracks.push_back(newTrackData); 
      }
    }
      
  }
    
  void PlaylistComponent::saveTrackPlaylist()
  {
    std::ofstream playlistCSV("Playlist.csv");
    for (TrackData& t : tracks)
    {
    playlistCSV << t.file.getFullPathName() << "," << t.length << "\n";
    }
  }
  void PlaylistComponent::loadTrackPlaylist()
  {
    std::ifstream playlistCSV("Playlist.csv");
    std::string filePath;
    std::string length;
    if(playlistCSV.is_open())
    {
      while (getline(playlistCSV, filePath, ',')) 
      {
        juce::File file{ filePath };
        TrackData newTrackData{ file };
        getline(playlistCSV, length);
        newTrackData.length = length;
        tracks.push_back(newTrackData);
      }
      playlistCSV.close();
    }
  }
  bool checkDuplicateTrack(juce::String fileNameWithoutExtension)
  {
   return false; 
   //(std::find(tracks.begin(), tracks.end(), fileNameWithoutExtension) != tracks.end());
   
  }
  void PlaylistComponent::deleteTrack(int id)
  {
    tracks.erase(tracks.begin() + id);
  }
   void loadDeck(DeckGUI* deckGUI)
   {

   }
  
  
  String PlaylistComponent::getLength(juce::URL audioURL)
  {
    playerForParsingMetaData->loadURL(audioURL);
    double seconds{ playerForParsingMetaData->getLengthInSeconds() };
    String minutes{ secondsToMinutes(seconds) };
    return minutes;

  }   
  String PlaylistComponent::secondsToMinutes(double seconds)
  {
    int secondsRounded{ int(std::round(seconds)) };
    String min{ std::to_string(secondsRounded / 60) };
    String sec{ std::to_string(secondsRounded % 60) };
    if (sec.length() < 2) 
    {
        sec = sec.paddedLeft('0', 2);
    }
    return String{ min + ":" + sec };
  }


  

