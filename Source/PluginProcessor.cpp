/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

AudioProcessorValueTreeState::ParameterLayout CompressorAudioProcessor::createParameterLayout()
{
	std::vector<std::unique_ptr<RangedAudioParameter>> params;

	{
		using FloatParamPair = std::pair<Identifier, AudioParameterFloat*&>;

		for (auto p : { FloatParamPair(Parameters::threshold, threshold),
						FloatParamPair(Parameters::ratio, ratio),
						FloatParamPair(Parameters::attack, attack),
			            FloatParamPair(Parameters::release, release),
						FloatParamPair(Parameters::gain, gain) })
		{
			auto& info = Parameters::parameterInfoMap[p.first];
			auto param = std::make_unique<AudioParameterFloat>(p.first.toString(), info.labelName, NormalisableRange<float>(info.lowerLimit, info.upperLimit), info.defaultValue);

			p.second = param.get();
			params.push_back(std::move(param));
		}
	}

	return { params.begin(), params.end() };
}

//==============================================================================
CompressorAudioProcessor::CompressorAudioProcessor()
     : state(*this, nullptr, "PARAMETERS", createParameterLayout()),
#ifndef JucePlugin_PreferredChannelConfigurations
AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
	//undoManager = new UndoManager();
	//state = new AudioProcessorValueTreeState(*this, undoManager);

	//state->createAndAddParameter(paramThreshold, "threshold", TRANS("threshold"), NormalisableRange <float>(-80.f, 0.0f, 0.01f), 0.0f, nullptr, nullptr);
	//state->createAndAddParameter(paramRatio, "ratio", TRANS("ratio"), NormalisableRange <float>(1.f, 50.0f, 1.f), 1.0f, nullptr, nullptr);
	//state->createAndAddParameter(paramAttack, "attack", TRANS("attack"), NormalisableRange <float>(0.01f, 1000.0f, 0.01f), 100.0f, nullptr, nullptr);
	//state->createAndAddParameter(paramRelease, "release", TRANS("release"), NormalisableRange <float>(10.f, 10000.0f, 0.01f), 100.0f, nullptr, nullptr);
	//state->createAndAddParameter(paramGain, "gain", TRANS("gain"), NormalisableRange <float>(0.f, 12.0f, 0.01f), 0.f, nullptr, nullptr);
	//state->addParameterListener(paramThreshold, this);
	//state->addParameterListener(paramRatio, this);
	//state->addParameterListener(paramAttack, this);
	//state->addParameterListener(paramThreshold, this);
	//state->addParameterListener(paramGain, this);

	//state->state = ValueTree("Compressor");
}

CompressorAudioProcessor::~CompressorAudioProcessor()
{

}

//==============================================================================
const String CompressorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CompressorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CompressorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool CompressorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double CompressorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CompressorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int CompressorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CompressorAudioProcessor::setCurrentProgram (int index)
{
}

const String CompressorAudioProcessor::getProgramName (int index)
{
    return {};
}

void CompressorAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void CompressorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	theCompressor.ballisticsFilter.initProcessing(sampleRate);

	updateProcessing();
	reset();

	isActive = true;
}

void CompressorAudioProcessor::reset()
{
	theCompressor.ballisticsFilter.reset();
	theCompressor.makeupgain.reset(sampleRate, 0.05);
}

void CompressorAudioProcessor::releaseResources()
{
	reset();
}

//void CompressorAudioProcessor::parameterChanged(const String &parameterID, float newValue)
//{
//	if (parameterID == Parameters::threshold.toString()) {
//		*threshold = newValue;
//		mustUpdateProcessing = true;
//		return;
//	}
//	if (parameterID == Parameters::ratio.toString()) {
//		*ratio = newValue;
//		mustUpdateProcessing = true;
//		return;
//	}
//	if (parameterID == Parameters::attack.toString()) {
//		*attack = newValue;
//		mustUpdateProcessing = true;
//		return;
//	}
//	if (parameterID == Parameters::release.toString()) {
//		*release = newValue;
//		mustUpdateProcessing = true;
//		return;
//	}
//	if (parameterID == Parameters::gain.toString()) {
//		*gain = newValue;
//		mustUpdateProcessing = true;
//		return;
//	}
//}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CompressorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void CompressorAudioProcessor::updateProcessing()
{
	//mustUpdateProcessing = false;

	theCompressor.thrlin = (float)(Decibels::decibelsToGain((float)*threshold));
	theCompressor.ratioinv = 1.f / (float)(*ratio);
	theCompressor.makeupgain.setValue((float)(Decibels::decibelsToGain((float)*gain)));

	theCompressor.ballisticsFilter.setAttackTime((float)(*attack));
	theCompressor.ballisticsFilter.setReleaseTime((float)(*release));

	parametersNeedUpdating.clear();
}

void CompressorAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    { 
        auto* bufferWritePtr = buffer.getWritePointer (channel);
		auto sideInput = 0.f;

		for (int i = 0; i < buffer.getNumSamples(); i++)
		{
			float cleansig = *bufferWritePtr;

			if (parametersNeedUpdating.test_and_set())
				updateProcessing();

			sideInput = jmax(sideInput, *bufferWritePtr);

			auto env = theCompressor.ballisticsFilter.processSingleSample(sideInput);

			auto cv = (env <= theCompressor.thrlin ? 1.f : std::pow(env / theCompressor.thrlin, theCompressor.ratioinv - 1.f));

			cv *= theCompressor.makeupgain.getNextValue();

			*bufferWritePtr = cleansig * cv;
			bufferWritePtr++;
		}
    }
}

AudioProcessorValueTreeState& CompressorAudioProcessor::getValueTreeState()
{
	return state;
}

//==============================================================================
bool CompressorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* CompressorAudioProcessor::createEditor()
{
    return new CompressorAudioProcessorEditor (*this);
}

//==============================================================================
void CompressorAudioProcessor::getStateInformation (MemoryBlock& destData)
{
	MemoryOutputStream stream(destData, false);
	state.state.writeToStream(stream);
}

void CompressorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
	ValueTree tree = ValueTree::readFromData(data, sizeInBytes);
	if (tree.isValid()) {
		state.state = tree;
	}
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CompressorAudioProcessor();
}
