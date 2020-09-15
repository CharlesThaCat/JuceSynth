/*
  ==============================================================================

    Envelope.h
    Created: 3 Sep 2020 4:16:32pm
    Author:  bhxxl

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class Envelope  : public juce::Component
{
public:
    Envelope(SynthFrameworkAudioProcessor&);
    ~Envelope() override;

    void paint (juce::Graphics&) override;
    void resized() override;


private:
	SynthFrameworkAudioProcessor& audioProcessor;

	juce::Slider attackSlider;
	juce::Slider decaySlider;
	juce::Slider sustainSlider;
	juce::Slider releaseSlider;
	
	juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> attackVal;
	juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> decayVal;
	juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> sustainVal;
	juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> releaseVal;
	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Envelope)
};
