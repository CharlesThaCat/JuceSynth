/*
  ==============================================================================

    SynthSound.h
    Created: 29 Aug 2020 11:40:52am
    Author:  bhxxl

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
public:
	//==============================================================================
	bool appliesToNote(int midiNoteNumber)
	{
		return true;
	}

	bool appliesToChannel(int midiChannel)
	{
		return true;
	}
};