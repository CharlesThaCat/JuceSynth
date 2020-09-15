/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthFrameworkAudioProcessorEditor::SynthFrameworkAudioProcessorEditor (SynthFrameworkAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), oscGui(p), osc2Gui(p), envGui(p), filterGui(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 200);

	addAndMakeVisible(oscGui);
	addAndMakeVisible(osc2Gui);
	addAndMakeVisible(envGui);
	addAndMakeVisible(filterGui);
}

SynthFrameworkAudioProcessorEditor::~SynthFrameworkAudioProcessorEditor()
{
}

//==============================================================================
void SynthFrameworkAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void SynthFrameworkAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

	// Code in this function all related to the (400, 200) main window

	const int componentWidth = 200;
	const int componentHeight = 200;
	juce::Rectangle<int> area = getLocalBounds().removeFromTop(componentHeight);
	oscGui.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
	osc2Gui.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
	filterGui.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));
	envGui.setBounds(area.removeFromLeft(componentWidth).removeFromTop(componentHeight));

}


