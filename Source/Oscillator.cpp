/*
  ==============================================================================

    Oscillator.cpp
    Created: 3 Sep 2020 11:12:49am
    Author:  bhxxl

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Oscillator.h"

//==============================================================================
Oscillator::Oscillator(SynthFrameworkAudioProcessor& p) : audioProcessor(p)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

	setSize(200, 200); // set the size of oscillator window 

	oscMenu.addItem("Sine", 1);
	oscMenu.addItem("Saw", 2);
	oscMenu.addItem("Square", 3);
	oscMenu.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(oscMenu);

	oscSelection = new juce::AudioProcessorValueTreeState::ComboBoxAttachment(audioProcessor.tree, "wavetype", oscMenu);
}

Oscillator::~Oscillator()
{
}

void Oscillator::paint (juce::Graphics& g)
{
	juce::Rectangle<int> titleArea(0, 10, getWidth(), 20);
	g.setColour(juce::Colours::white);
	g.drawText("Oscillator 1", titleArea, juce::Justification::centredTop);
}

void Oscillator::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

	// Code in this function all related to the (200, 200) oscillator window
	
	juce::Rectangle<int> area = getLocalBounds().reduced(40);

	oscMenu.setBounds(area.removeFromTop(20));
}

