/*
  ==============================================================================

    SynthVoice.h
    Created: 29 Aug 2020 11:41:15am
    Author:  bhxxl

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "maximilian.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
	// Osc ==============================================================================
	void getOscType(float* oscSelection)
	{
		// get osc type from plugin processor
		theWaveType = *oscSelection; // an int tag specifying the osc type
	}

	double setOscType()
	{
		// return wavetable osc according to osc type and current frequency
		if (theWaveType == 0)
		{
			return osc1.sinewave(frequency);
		}
		if (theWaveType == 1)
		{
			return osc1.saw(frequency);
		}
		if (theWaveType == 2)
		{
			return osc1.square(frequency);
		}
		else
		{
			return osc1.sinewave(frequency);
		}
	}

	// ADSR ==============================================================================
	void setEnvelopeParam(float* attack, float* decay, float* sustain, float* release)
	{
		// get & set adsr from plugin processor
		env1.setAttack(double(*attack));
		env1.setDecay(double(*decay));
		env1.setSustain(double(*sustain));
		env1.setRelease(double(*release));
	}

	double getEnvelope()
	{
		// return the wave after osc and adsr and filter (filtering done in PluginProcessor with juce dsp module)
		return env1.adsr(setOscType(), env1.trigger);
	}

	//// Filter ==============================================================================
	//void getFilterParam(float* filterSelection, float* filterCutoff, float* filterResonance)
	//{
	//	// get filter parameters from plugin processor
	//	theFilterType = *filterSelection;
	//	cutoff = *filterCutoff;
	//	resonance = *filterResonance;
	//}

	//double getFilteredEnvelope()
	//{
	//	// return the wave after osc and adsr and filter
	//	if (theFilterType == 0)
	//	{
	//		return filter1.lores(getEnvelope(), cutoff, resonance);
	//	}
	//	if (theFilterType == 1)
	//	{
	//		return filter1.hires(getEnvelope(), cutoff, resonance);
	//	}
	//	if (theFilterType == 2)
	//	{
	//		return filter1.bandpass(getEnvelope(), cutoff, resonance);
	//	}
	//	else
	//	{
	//		return filter1.lores(getEnvelope(), cutoff, resonance);
	//	}
	//}

	// System functions ==============================================================================
	bool canPlaySound(juce::SynthesiserSound* sound)
	{
		return dynamic_cast<SynthSound*>(sound) != nullptr;
	}

	void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) 
		// what to do when midi key is pressed
	{
		env1.trigger = 1; // a flag to show that the envelope is triggered (taking into effect)

		frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber); // get the frequency of the pressed midi note
		juce::Logger::outputDebugString(std::to_string(midiNoteNumber)); 

		level = velocity; // get the velocity of the pressed midi note

	}

	void stopNote(float velocity, bool allowTailOff)
		// what to do when midi key is released
	{
		env1.trigger = 0; // a flag to show that the envelope is out of effect
		allowTailOff = true;

		if (velocity == 0)
			clearCurrentNote(); // if no velocity on a certain note, then we kick that note out and spare an available space
	}

	void pitchWheelMoved(int newPitchWheelValue)
	{

	}

	void controllerMoved(int controllerNumber, int newControllerValue)
	{

	}

	void renderNextBlock(juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples)
	{
	
		for (int sample = 0; sample < numSamples; ++sample)
		{
			double theSound = getEnvelope();
			
			for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
			{
				// add values to the buffer
				outputBuffer.addSample(channel, startSample, theSound * level);
			}

			++startSample;
		}
	}

private:
	double level; // velocity of a midi note
	double frequency; // frequency of a midi note
	int theWaveType; // osc type
	//int theFilterType; // filter type
	//float cutoff; // filter cutoff frequency
	//float resonance; // filter resonance

	maxiOsc osc1; // Maximilian oscillator 1
	maxiEnv env1;
	//maxiFilter filter1;

};