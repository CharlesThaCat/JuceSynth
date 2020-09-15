/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthFrameworkAudioProcessor::SynthFrameworkAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
	attackTime(0.1f),
	tree(*this, nullptr)
#endif
{
	using Parameter = juce::AudioProcessorValueTreeState::Parameter;

	juce::NormalisableRange<float> attackParam(0.1f, 5000.0f);
	juce::NormalisableRange<float> decayParam(0.1f, 5000.0f);
	juce::NormalisableRange<float> sustainParam(0.1f, 1.0f);
	juce::NormalisableRange<float> releaseParam(0.1f, 5000.0f);
	tree.createAndAddParameter(std::make_unique<Parameter>("attack", "ATTACK", "ATTACK", attackParam, 0.1f, nullptr, nullptr));
	tree.createAndAddParameter(std::make_unique<Parameter>("decay", "DECAY", "DECAY", decayParam, 0.1f, nullptr, nullptr));
	tree.createAndAddParameter(std::make_unique<Parameter>("sustain", "SUSTAIN", "SUSTAIN", sustainParam, 0.1f, nullptr, nullptr));
	tree.createAndAddParameter(std::make_unique<Parameter>("release", "RELEASE", "RELEASE", releaseParam, 0.1f, nullptr, nullptr));

	juce::NormalisableRange<float> wavetypeParam(0.0f, 2.0f);
	tree.createAndAddParameter(std::make_unique<Parameter>("wavetype", "WAVETYPE", "WAVETYPE", wavetypeParam, 0.0f, nullptr, nullptr));

	juce::NormalisableRange<float> wave2typeParam(0.0f, 2.0f);
	tree.createAndAddParameter(std::make_unique<Parameter>("wave2type", "WAVE2TYPE", "WAVE2TYPE", wave2typeParam, 0.0f, nullptr, nullptr));

	juce::NormalisableRange<float> blendParam(0.0f, 1.0f);
	tree.createAndAddParameter(std::make_unique<Parameter>("blend", "BLEND", "BLEND", blendParam, 0.6f, nullptr, nullptr));

	juce::NormalisableRange<float> cutoffParam(20.0f, 10000.0f);
	juce::NormalisableRange<float> resonanceParam(1.0, 5.0f); 
	juce::NormalisableRange<float> filtertypeParam(0.0f, 2.0f);
	tree.createAndAddParameter(std::make_unique<Parameter>("cutoff", "CUTOFF", "CUTOFF", cutoffParam, 400.0f, nullptr, nullptr));
	tree.createAndAddParameter(std::make_unique<Parameter>("resonance", "RESONANCE", "RESONANCE", resonanceParam, 1.0f, nullptr, nullptr));
	tree.createAndAddParameter(std::make_unique<Parameter>("filtertype", "FILTERTYPE", "FILTERTYPE", filtertypeParam, 0.0f, nullptr, nullptr));


	tree.state = juce::ValueTree("Foo");




	// create a polyphonic synthesizer available up to 5 voices
	mySynth.clearVoices(); // clear existed voice before create something new
	for (int i = 0; i < 5; i++)
	{
		mySynth.addVoice(new SynthVoice());
	}

	mySynth.clearSounds();
	mySynth.addSound(new SynthSound());

}

SynthFrameworkAudioProcessor::~SynthFrameworkAudioProcessor()
{
}

//==============================================================================
const juce::String SynthFrameworkAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SynthFrameworkAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SynthFrameworkAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SynthFrameworkAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SynthFrameworkAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SynthFrameworkAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SynthFrameworkAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SynthFrameworkAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SynthFrameworkAudioProcessor::getProgramName (int index)
{
    return {};
}

void SynthFrameworkAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SynthFrameworkAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

	lastSampleRate = sampleRate; // just a good practice here in case we change a sound card then the sample rate would change
	mySynth.setCurrentPlaybackSampleRate(lastSampleRate);

	juce::dsp::ProcessSpec spec;
	spec.sampleRate = lastSampleRate;
	spec.maximumBlockSize = samplesPerBlock;
	spec.numChannels = getTotalNumOutputChannels();
	stateVariableFilter.reset();
	stateVariableFilter.prepare(spec);
	updateFilter();

}

void SynthFrameworkAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SynthFrameworkAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SynthFrameworkAudioProcessor::updateFilter()
{
	int filterChoice = *tree.getRawParameterValue("filtertype");
	int cutoffChoice = *tree.getRawParameterValue("cutoff");
	int resonanceChoice = *tree.getRawParameterValue("resonance");

	if (filterChoice == 0)
	{
		stateVariableFilter.state->type = juce::dsp::StateVariableFilter::Parameters<float>::Type::lowPass;
		stateVariableFilter.state->setCutOffFrequency(lastSampleRate, cutoffChoice, resonanceChoice);
	}
	if (filterChoice == 1)
	{
		stateVariableFilter.state->type = juce::dsp::StateVariableFilter::Parameters<float>::Type::highPass;
		stateVariableFilter.state->setCutOffFrequency(lastSampleRate, cutoffChoice, resonanceChoice);
	}
	if (filterChoice == 2)
	{
		stateVariableFilter.state->type = juce::dsp::StateVariableFilter::Parameters<float>::Type::bandPass;
		stateVariableFilter.state->setCutOffFrequency(lastSampleRate, cutoffChoice, resonanceChoice);
	}
}

void SynthFrameworkAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	for (int i = 0; i < mySynth.getNumVoices(); i++)
	{
		if (myVoice = dynamic_cast<SynthVoice*>(mySynth.getVoice(i)))
		{
			float* ptrAttack = (float*)tree.getRawParameterValue("attack");
			float* ptrDecay = (float*)tree.getRawParameterValue("decay");
			float* ptrSustain = (float*)tree.getRawParameterValue("sustain");
			float* ptrRelease = (float*)tree.getRawParameterValue("release");
			myVoice->setEnvelopeParam(ptrAttack, ptrDecay, ptrSustain, ptrRelease);

			float* ptrOsc = (float*)tree.getRawParameterValue("wavetype");
			myVoice->getOscType(ptrOsc);

			float* ptrOsc2 = (float*)tree.getRawParameterValue("wave2type");
			myVoice->getOsc2Type(ptrOsc2);

			float* ptrBlend = (float*)tree.getRawParameterValue("blend");
			myVoice->getOscBlend(ptrBlend);

			/*float* ptrFilter = (float*)tree.getRawParameterValue("filtertype");
			float* ptrCutoff = (float*)tree.getRawParameterValue("cutoff");
			float* ptrResonance = (float*)tree.getRawParameterValue("resonance");
			myVoice->getFilterParam(ptrFilter, ptrCutoff, ptrResonance);*/
		}
	}

	buffer.clear();

	mySynth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
	updateFilter();

	juce::dsp::AudioBlock<float> block(buffer);
	stateVariableFilter.process(juce::dsp::ProcessContextReplacing<float>(block));


    //juce::ScopedNoDenormals noDenormals;
    //auto totalNumInputChannels  = getTotalNumInputChannels();
    //auto totalNumOutputChannels = getTotalNumOutputChannels();

    //// In case we have more outputs than inputs, this code clears any output
    //// channels that didn't contain input data, (because these aren't
    //// guaranteed to be empty - they may contain garbage).
    //// This is here to avoid people getting screaming feedback
    //// when they first compile a plugin, but obviously you don't need to keep
    //// this code if your algorithm always overwrites all the output channels.
    //for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    //    buffer.clear (i, 0, buffer.getNumSamples());

    //// This is the place where you'd normally do the guts of your plugin's
    //// audio processing...
    //// Make sure to reset the state if your inner loop is processing
    //// the samples and the outer loop is handling the channels.
    //// Alternatively, you can process the samples with the channels
    //// interleaved by keeping the same state.
    //for (int channel = 0; channel < totalNumInputChannels; ++channel)
    //{
    //    auto* channelData = buffer.getWritePointer (channel);

    //    // ..do something to the data...
    //}
}

//==============================================================================
bool SynthFrameworkAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SynthFrameworkAudioProcessor::createEditor()
{
    return new SynthFrameworkAudioProcessorEditor (*this);
}

//==============================================================================
void SynthFrameworkAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SynthFrameworkAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SynthFrameworkAudioProcessor();
}
