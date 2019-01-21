/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class CompressorAudioProcessorEditor  : public AudioProcessorEditor,
										private AudioProcessorValueTreeState::Listener,
										private ChangeListener
	                                    
{
public:
    CompressorAudioProcessorEditor (CompressorAudioProcessor&);
    ~CompressorAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

	void changeListenerCallback(ChangeBroadcaster* sender) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    CompressorAudioProcessor& processor;

	void parameterChanged(const String&, float) override;

	struct RotarySliderWithLabel : public Component
	{
		RotarySliderWithLabel(const Identifier& paramID)
			: identifier(paramID),
			  slider(identifier.toString()),
			  label(identifier.toString(), Parameters::parameterInfoMap[paramID].labelName)
		{
			slider.setSliderStyle(Slider::Rotary);
			slider.setTextBoxStyle(Slider::TextBoxBelow, false, slider.getTextBoxWidth(), slider.getTextBoxHeight());
			addAndMakeVisible(slider);

			label.setJustificationType(Justification::centred);
			addAndMakeVisible(label);

		}

		void resized() override
		{
			auto bounds = getLocalBounds();

			label.setBounds(bounds.removeFromTop(10));
			slider.setBounds(bounds);
		}

		Identifier identifier;
		Slider slider;
		Label label;
	};

	juce::Rectangle<int>          plotFrame;

	void addFloatParameter(const Identifier&);
	void addParameterListeners();

	using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;

	OwnedArray<RotarySliderWithLabel> parameterSliders;
	OwnedArray<SliderAttachment> parameterSliderAttachments;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompressorAudioProcessorEditor)
};
