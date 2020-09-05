/*
  ==============================================================================

    Envelope.cpp
    Created: 3 Sep 2020 4:16:32pm
    Author:  bhxxl

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Envelope.h"

//==============================================================================
Envelope::Envelope(SynthFrameworkAudioProcessor& p) : audioProcessor(p)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

	setSize(200, 200); // set the size of adsr window 

	addAndMakeVisible(attackSlider);
	attackSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	attackSlider.setRange(0.1, 5000.0);
	attackSlider.setValue(0.1);
	attackSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
	attackVal = new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.tree, "attack", attackSlider);

	addAndMakeVisible(decaySlider);
	decaySlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	decaySlider.setRange(0.1, 5000.0);
	decaySlider.setValue(0.1);
	decaySlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
	decayVal = new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.tree, "decay", decaySlider);

	addAndMakeVisible(sustainSlider);
	sustainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	sustainSlider.setRange(0.1, 5000.0);
	sustainSlider.setValue(0.1);
	sustainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
	sustainVal = new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.tree, "sustain", sustainSlider);

	addAndMakeVisible(releaseSlider);
	releaseSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	releaseSlider.setRange(0.1, 5000.0);
	releaseSlider.setValue(0.1);
	releaseSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
	releaseVal = new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.tree, "release", releaseSlider);
	

}

Envelope::~Envelope()
{
}

void Envelope::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */
    
	juce::Rectangle<int> titleArea(0, 10, getWidth(), 20);
	g.setColour(juce::Colours::white);
	g.drawText("Envelope", titleArea, juce::Justification::centredTop);

}

void Envelope::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
	
	// Code in this function all related to the (200, 200) envelope window

	juce::Rectangle<int> area = getLocalBounds().reduced(50);

	int sliderWidth = 25;
	int sliderHeight = 175;

	attackSlider.setBounds(area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight).withTrimmedTop(10));
	decaySlider.setBounds(area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight).withTrimmedTop(10));
	sustainSlider.setBounds(area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight).withTrimmedTop(10));
	releaseSlider.setBounds(area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight).withTrimmedTop(10));

}

