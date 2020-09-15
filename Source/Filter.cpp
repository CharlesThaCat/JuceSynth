/*
  ==============================================================================

    Filter.cpp
    Created: 4 Sep 2020 9:32:45am
    Author:  bhxxl

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Filter.h"

//==============================================================================
Filter::Filter(SynthFrameworkAudioProcessor& p) : audioProcessor(p)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

	setSize(200, 200); // set the size of filter window 

	addAndMakeVisible(cutoffSlider);
	cutoffSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
	cutoffSlider.setRange(20.0, 10000.0);
	cutoffSlider.setValue(400.0);
	cutoffSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
	cutoffSlider.setSkewFactorFromMidPoint(2000.0); // set scale
	cutoffVal = new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.tree, "cutoff", cutoffSlider);

	addAndMakeVisible(resonanceSlider);
	resonanceSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
	resonanceSlider.setRange(1.0, 5.0);
	resonanceSlider.setValue(1.0);
	resonanceSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
	resonanceVal = new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.tree, "resonance", resonanceSlider);

	filterMenu.addItem("Lowpass", 1);
	filterMenu.addItem("Highpass", 2);
	filterMenu.addItem("Bandpass", 3);
	filterMenu.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(filterMenu);
	filterSelection = new juce::AudioProcessorValueTreeState::ComboBoxAttachment(audioProcessor.tree, "filtertype", filterMenu);

}

Filter::~Filter()
{
}

void Filter::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

	juce::Rectangle<int> titleArea(0, 10, getWidth(), 20);
	g.setColour(juce::Colours::white);
	g.drawText("Filter", titleArea, juce::Justification::centredTop);

}

void Filter::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

	juce::Rectangle<int> area = getLocalBounds().reduced(40);

	filterMenu.setBounds(area.removeFromTop(20));
	cutoffSlider.setBounds(30, 100, 70, 70);
	resonanceSlider.setBounds(100, 100, 70, 70);
}
