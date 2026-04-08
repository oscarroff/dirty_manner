#pragma once

#include "PluginProcessor.h"
#include "GraphComponent.h"

//==============================================================================
class AudioPluginAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&);
    ~AudioPluginAudioProcessorEditor() override;
    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void updateStatusText(const std::string status);
    void updateGraphData(const std::vector<float>& newData);


private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor& processorRef;
    juce::Label titleLabel;
    juce::Label inputText;
    juce::Label statusText;
    juce::Label infoText;
    GraphComponent graphComponent;
    std::vector<float> exampleData;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
