/*
  ==============================================================================

    Oscillator2.cpp
    Created: 14 Sep 2020 8:07:04pm
    Author:  bhxxl

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Oscillator2.h"

//==============================================================================
Oscillator2::Oscillator2(SynthFrameworkAudioProcessor& p) : audioProcessor(p)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	setSize(200, 200); // set the size of oscillator window 

	osc2Menu.addItem("Triangle", 1);
	osc2Menu.addItem("Saw", 2);
	osc2Menu.addItem("Square", 3);
	osc2Menu.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(osc2Menu);

	osc2Selection = new juce::AudioProcessorValueTreeState::ComboBoxAttachment(audioProcessor.tree, "wave2type", osc2Menu);


	blendSlider.setSliderStyle(juce::Slider::LinearVertical);
	blendSlider.setRange(0.0f, 1.0f);
	blendSlider.setValue(1.0f);
	blendSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
	addAndMakeVisible(blendSlider);

	blendVal = new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.tree, "blend", blendSlider);
}

Oscillator2::~Oscillator2()
{
}

void Oscillator2::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

	juce::Rectangle<int> titleArea(0, 10, getWidth(), 20);
	g.setColour(juce::Colours::white);
	g.drawText("Oscillator 2", titleArea, juce::Justification::centredTop);
}

void Oscillator2::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

	juce::Rectangle<int> area = getLocalBounds().reduced(40);
	osc2Menu.setBounds(area.removeFromTop(20));

	int sliderWidth = 25;
	int sliderHeight = 175;
	blendSlider.setBounds(area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight).withTrimmedTop(10));
}
