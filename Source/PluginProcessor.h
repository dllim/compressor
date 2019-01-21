/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <map>
#include "DSP.h"

//==============================================================================
/**
*/

namespace Parameters
{
	static const Identifier threshold	{ "threshold" };
	static const Identifier ratio		{ "ratio" };
	static const Identifier attack		{ "attack" };
	static const Identifier release		{ "release" };
	static const Identifier gain        { "gain" };

	struct ParameterInfo
	{
		String labelName;
		float defaultValue;
		float lowerLimit;
		float upperLimit;
	};

	static std::map<Identifier, ParameterInfo> parameterInfoMap
	{
		{	threshold, {  "Threshold", 0.f, -80.f, 0.f  }	},
		{	ratio,     {  "Ratio", 1.f, 1.f, 16.f  }	},
		{	attack,    {  "Attack", 100.f, 0.f, 1000.f  }	},
		{	release,   {  "Release", 100.f, 10.f, 10000.f  }	},
		{	gain,      {  "Gain", 0.f, 0.f, 12.f  }	}
	};
}

class CompressorAudioProcessor  : public AudioProcessor,
			                      
	                              public ChangeBroadcaster
{
public:
    //==============================================================================
    CompressorAudioProcessor();
    ~CompressorAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	//void parameterChanged(const String & parameterId, float newValue);

	void updateProcessing();

	AudioProcessorValueTreeState& getValueTreeState();

	//bool mustUpdateProcessing = false;

	void setParametersNeedUpdating() { parametersNeedUpdating.test_and_set(); }

private:

	AudioParameterFloat* threshold = nullptr;
	AudioParameterFloat* ratio = nullptr;
	AudioParameterFloat* attack = nullptr;
	AudioParameterFloat* release = nullptr;
	AudioParameterFloat* gain = nullptr;

	AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

	void reset();

	AudioProcessorValueTreeState state;
	ScopedPointer<UndoManager> undoManager;

	double sampleRate = 44100.0;

	struct CompressorState
	{
		float thrlin, ratioinv;
		LinearSmoothedValue<float> makeupgain;
		Ballistics ballisticsFilter;
		bool mustUseBallistics;
	};

	CompressorState theCompressor;
	bool isActive = false;

	std::atomic_flag parametersNeedUpdating  { true };
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompressorAudioProcessor)
};
