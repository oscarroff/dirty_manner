#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);

    addAndMakeVisible (titleLabel);
    titleLabel.setFont (juce::Font (16.0f, juce::Font::bold));
    titleLabel.setText ("input a date*", juce::dontSendNotification);
    titleLabel.setColour (juce::Label::textColourId, juce::Colours::lightgreen);
    titleLabel.setJustificationType (juce::Justification::centred);

    addAndMakeVisible (statusText);
    statusText.setColour (juce::Label::backgroundColourId, juce::Colours::black);
    inputText.setJustificationType (juce::Justification::centred);

    addAndMakeVisible (inputText);
    inputText.setText ("DD/MM/YYYY", juce::dontSendNotification);
    inputText.setEditable (true);
    inputText.setColour (juce::Label::backgroundColourId, juce::Colours::darkblue);
    inputText.onTextChange = [this] { statusText.setText (inputText.getText().toUpperCase(), juce::dontSendNotification); };
    inputText.setJustificationType (juce::Justification::centred);

    addAndMakeVisible (infoText);
    infoText.setFont (juce::Font (12.0f));
    infoText.setText ("PM2.5 air quality: 1 week at 1 hour intervals", juce::dontSendNotification);
    infoText.setColour (juce::Label::textColourId, juce::Colours::lightgreen);
    infoText.setJustificationType (juce::Justification::centred);

    addAndMakeVisible(graphComponent);
    
    // Example data - replace with your actual data
    exampleData = { 0.0f };
    graphComponent.setData(exampleData.data(), exampleData.size());

    setSize (320, 280);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

// Fill the window with a canvas
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

// Layout
void AudioPluginAudioProcessorEditor::resized()
{
    titleLabel   .setBounds (10,  10, getWidth() - 20,  30);
    inputText    .setBounds (110, 50, getWidth() - 220, 20);
    statusText.setBounds (110, 80, getWidth() - 220, 20);
    graphComponent.setBounds(10, 95, getWidth() - 20, getHeight() - 120);
    infoText   .setBounds (10,  getHeight() - 30, getWidth() - 20,  20);
}

// Public method to indicate bad input
void updateStatusText(const std::string status)
{
    this->statusText.setText(status, juce::dontSendNotification);
}

// Public method to update graph data
void updateGraphData(const std::vector<float>& newData)
{
    exampleData = newData;
    graphComponent.setData(exampleData.data(), exampleData.size());
}
