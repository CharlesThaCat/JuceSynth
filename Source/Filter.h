/*
  ==============================================================================

    Filter.h
    Created: 4 Sep 2020 9:32:45am
    Author:  bhxxl

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class Filter  : public juce::Component
{
public:
    Filter(SynthFrameworkAudioProcessor&);
    ~Filter() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
	SynthFrameworkAudioProcessor& audioProcessor;

	juce::Slider cutoffSlider;
	juce::Slider resonanceSlider;
	juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> cutoffVal;
	juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> resonanceVal;

	juce::ComboBox filterMenu;
	juce::ScopedPointer<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterSelection;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Filter)
};
