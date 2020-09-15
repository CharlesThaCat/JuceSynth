/*
  ==============================================================================

    Oscillator2.h
    Created: 14 Sep 2020 8:07:04pm
    Author:  bhxxl

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class Oscillator2  : public juce::Component
{
public:
    Oscillator2(SynthFrameworkAudioProcessor&);
    ~Oscillator2() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
	SynthFrameworkAudioProcessor& audioProcessor;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Oscillator2)

	juce::ComboBox osc2Menu;
	juce::Slider blendSlider; // blend between osc1 and osc2

	juce::ScopedPointer<juce::AudioProcessorValueTreeState::ComboBoxAttachment> osc2Selection;
	juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> blendVal;
};
