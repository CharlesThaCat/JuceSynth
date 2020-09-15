/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Oscillator.h"
#include "Oscillator2.h"
#include "Envelope.h"
#include "Filter.h"

//==============================================================================
/**
*/
class SynthFrameworkAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SynthFrameworkAudioProcessorEditor (SynthFrameworkAudioProcessor&);
    ~SynthFrameworkAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

	
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SynthFrameworkAudioProcessor& audioProcessor;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthFrameworkAudioProcessorEditor)

	Oscillator oscGui;
	Oscillator2 osc2Gui;
	Envelope envGui;
	Filter filterGui;
	
};
