/*
  ==============================================================================

    Oscillator.h
    Created: 3 Sep 2020 11:12:49am
    Author:  bhxxl

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class Oscillator  : public juce::Component
{
public:
    Oscillator(SynthFrameworkAudioProcessor&);
    ~Oscillator() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	SynthFrameworkAudioProcessor& audioProcessor;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Oscillator)
	juce::ComboBox oscMenu;

	juce::ScopedPointer<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscSelection;
};
