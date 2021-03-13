/*
  ==============================================================================

    DeckGUI.cpp
    Created: 7 Mar 2021 4:30:49pm
    Author:  sapientex

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, 
                AudioFormatManager & 	formatManagerToUse,
                AudioThumbnailCache & 	cacheToUse
           ) : player(_player), 
               waveformDisplay(formatManagerToUse, cacheToUse)
{

    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
       
    addAndMakeVisible(volSlider);
    addAndMakeVisible (volLabel);

    addAndMakeVisible(speedSlider);
    addAndMakeVisible(speedLabel);

    addAndMakeVisible(posSlider);
    addAndMakeVisible(posLabel); 
    
   
    
    

    addAndMakeVisible(waveformDisplay);


    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);

    
   
    volSlider.addListener(this);
    volLabel.setText ("Volume", juce::dontSendNotification);
    volLabel.attachToComponent (&volSlider, true);
    volLabel.setColour (juce::Label::textColourId, juce::Colours::aquamarine);
    volLabel.setJustificationType (juce::Justification::left);
    
    speedSlider.addListener(this);
    speedLabel.setText ("Speed", juce::dontSendNotification);
    speedLabel.attachToComponent (&speedSlider, true);
    speedLabel.setColour (juce::Label::textColourId, juce::Colours::aquamarine);
    speedLabel.setJustificationType (juce::Justification::left);
    
    posSlider.addListener(this);
    posLabel.setText ("Position", juce::dontSendNotification);
    posLabel.attachToComponent (&posSlider, true);
    posLabel.setColour (juce::Label::textColourId, juce::Colours::aquamarine);
    posLabel.setJustificationType (juce::Justification::left);


    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 10.0);
    posSlider.setRange(0.0, 1.0);

    startTimer(500);


}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("DeckGUI", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void DeckGUI::resized()
{
    double rowH = getHeight() / 6;
    double columnH = getWidth() / 3 ; 
    waveformDisplay.setBounds(0, 0, getWidth(),rowH *2);
    volSlider.setBounds(columnH*0.25,rowH*2,getWidth(),rowH);
    speedSlider.setBounds(columnH*0.25,rowH*3,getWidth(),rowH);
    posSlider.setBounds(columnH*0.25,rowH*4,getWidth(),rowH);
    playButton.setBounds(0,rowH*5,columnH,rowH);
    stopButton.setBounds(columnH * 1,rowH*5,columnH,rowH);
    loadButton.setBounds(columnH * 2,rowH*5,columnH,rowH);
    
}

void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        std::cout << "Play button was clicked " << std::endl;
        player->start();
    }
     if (button == &stopButton)
    {
        std::cout << "Stop button was clicked " << std::endl;
        player->stop();

    }
    if (button == &loadButton)
    {
        FileChooser chooser{"Select a file..."};
        if (chooser.browseForFileToOpen())
        {
            player->loadURL(URL{chooser.getResult()});
            waveformDisplay.loadURL(URL{chooser.getResult()});
            
        }
    }
}

void DeckGUI::sliderValueChanged (Slider *slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
    
}

bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
  std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
  return true; 
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{
  std::cout << "DeckGUI::filesDropped" << std::endl;
  if (files.size() == 1)
  {
    loadAudio(URL{File{files[0]}});
  }
}

void DeckGUI::timerCallback()
{
    //std::cout << "DeckGUI::timerCallback" << std::endl;
    waveformDisplay.setPositionRelative(
            player->getPositionRelative());
}

 void DeckGUI::loadAudio(juce::URL audioURL)
{
    player->loadURL(audioURL);
    waveformDisplay.loadURL(audioURL);
}


    

