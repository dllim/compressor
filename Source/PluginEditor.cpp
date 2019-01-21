/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "DSP.h"

//==============================================================================
CompressorAudioProcessorEditor::CompressorAudioProcessorEditor (CompressorAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
	
	for (auto paramID : { Parameters::threshold, Parameters::ratio, Parameters::attack,
						Parameters::release, Parameters::gain })
		addFloatParameter(paramID);
	
	addParameterListeners();
	processor.addChangeListener(this);
	//startTimerHz(20);
    setSize (600, 450);
}

CompressorAudioProcessorEditor::~CompressorAudioProcessorEditor()
{
	processor.removeChangeListener(this);
}

//==============================================================================
void CompressorAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
	Graphics::ScopedSaveState state(g);

	g.setFont(12.0f);
	g.setColour(Colours::silver);
	g.drawRoundedRectangle(plotFrame.toFloat(), 5, 2);

	for (int i = 0; i < 13; ++i) {
		double x;
		//double totalLength = (3 + std::log10(2.2)) * 10;
		g.setColour(Colours::silver.withAlpha(0.3f));
		x = plotFrame.getX() + plotFrame.getWidth() * (i / 12.f);
		g.drawVerticalLine(roundToInt(x), plotFrame.getY(), plotFrame.getBottom());

		g.drawHorizontalLine(roundToInt(plotFrame.getY() + plotFrame.getHeight() * (i / 12.f)), plotFrame.getX(), plotFrame.getRight());
	}

	g.reduceClipRegion(plotFrame);
}

void CompressorAudioProcessorEditor::resized()
{
	auto bounds = getLocalBounds().reduced(13);

	auto paramSlice = bounds.removeFromBottom(250);

	auto getSliderForParameter = [this](const Identifier& paramID) -> RotarySliderWithLabel*
	{
		for (auto* s : parameterSliders)
			if (s->identifier == paramID)
				return s;

		jassertfalse;
		return nullptr;
	};

	for (auto paramID : { Parameters::threshold, Parameters::ratio, Parameters::attack, Parameters::release, Parameters::gain })
	{
		getSliderForParameter(paramID)->setBounds(paramSlice.removeFromLeft(100));
		paramSlice.removeFromLeft(5);
	}

    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

}

void CompressorAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster* sender)
{
	ignoreUnused(sender);
	repaint();
}


void CompressorAudioProcessorEditor::addFloatParameter(const Identifier& paramID)
{
	auto* sliderComp = parameterSliders.add(new RotarySliderWithLabel(paramID));
	addAndMakeVisible(sliderComp);

	parameterSliderAttachments.add(new SliderAttachment(processor.getValueTreeState(), paramID.toString(), sliderComp->slider));
}

void CompressorAudioProcessorEditor::addParameterListeners()
{
	auto& state = processor.getValueTreeState();

	state.addParameterListener (Parameters::threshold.toString(), this);
	state.addParameterListener (Parameters::ratio.toString(), this);
	state.addParameterListener (Parameters::attack.toString(), this);
	state.addParameterListener (Parameters::release.toString(), this);
	state.addParameterListener (Parameters::gain.toString(), this);
}

void CompressorAudioProcessorEditor::parameterChanged(const String &parameterID, float newValue)
{
	if (parameterID == Parameters::threshold.toString()) {
		processor.setParametersNeedUpdating();
		return;
	}
	if (parameterID == Parameters::ratio.toString()) {
		processor.setParametersNeedUpdating();
		return;
	}
	if (parameterID == Parameters::attack.toString()) {
		processor.setParametersNeedUpdating();
		return;
	}
	if (parameterID == Parameters::release.toString()) {
		processor.setParametersNeedUpdating();
		return;
	}
	if (parameterID == Parameters::gain.toString()) {
		processor.setParametersNeedUpdating();
		return;
	}
}