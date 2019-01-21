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
	/*thresholdSlider = new Slider(Slider::RotaryVerticalDrag, Slider::TextBoxBelow);
	ratioSlider = new Slider(Slider::RotaryVerticalDrag, Slider::TextBoxBelow);
	attackSlider = new Slider(Slider::RotaryVerticalDrag, Slider::TextBoxBelow);
	releaseSlider = new Slider(Slider::RotaryVerticalDrag, Slider::TextBoxBelow);
	gainSlider = new Slider(Slider::RotaryVerticalDrag, Slider::TextBoxBelow);

	thresholdLabel = new Label("", "Threshold");
	ratioLabel = new Label("", "Ratio");
	attackLabel = new Label("", "Attack");
	releaseLabel = new Label("", "Release");
	gainLabel = new Label("", "Gain");

	thresholdAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getValueTreeState(), CompressorAudioProcessor::paramThreshold, *thresholdSlider);
	ratioAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getValueTreeState(), CompressorAudioProcessor::paramRatio, *ratioSlider);
	attackAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getValueTreeState(), CompressorAudioProcessor::paramAttack, *attackSlider);
	releaseAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getValueTreeState(), CompressorAudioProcessor::paramRelease, *releaseSlider);
	gainAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getValueTreeState(), CompressorAudioProcessor::paramGain, *gainSlider);*/

	/*addAndMakeVisible(thresholdSlider);
	thresholdSlider->setRange(-80.0, 0.0);
	thresholdLabel->setJustificationType(Justification::centred);
	thresholdLabel->attachToComponent(thresholdSlider, false);

	addAndMakeVisible(ratioSlider);
	ratioSlider->setRange(1.0, 50.0);
	ratioSlider->setSkewFactor(0.2);
	ratioLabel->setJustificationType(Justification::centred);
	ratioLabel->attachToComponent(ratioSlider, false);

	addAndMakeVisible(attackSlider);
	attackSlider->setRange(0.1, 1000.0);
	attackSlider->setSkewFactor(0.2);
	attackLabel->setJustificationType(Justification::centred);
	attackLabel->attachToComponent(attackSlider, false);

	addAndMakeVisible(releaseSlider);
	releaseSlider->setRange(10.0, 10000.0);
	releaseSlider->setSkewFactor(0.2);
	releaseLabel->setJustificationType(Justification::centred);
	releaseLabel->attachToComponent(releaseSlider, false);

	addAndMakeVisible(gainSlider);
	gainSlider->setRange(0.0, 12.0);
	gainLabel->setJustificationType(Justification::centred);
	gainLabel->attachToComponent(gainSlider, false);*/

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
//	plotFrame = getLocalBounds().reduced(3, 3);
//	auto bandSpace = plotFrame.removeFromBottom(getHeight() / 2);
//	plotFrame.reduce(3, 3);
//
//	juce::Rectangle<int> thresholdbox(getLocalBounds());
//	thresholdbox.setWidth(getWidth() / 6);
//	thresholdbox.setHeight(getHeight() / 4);
//	thresholdbox.translate(20, 300);
////	thresholdSlider->setBounds(thresholdbox);
//
//	juce::Rectangle<int> ratiobox(getLocalBounds());
//	ratiobox.setWidth(getWidth() / 6);
//	ratiobox.setHeight(getHeight() / 4);
//	ratiobox.translate(120, 300);
////	ratioSlider->setBounds(ratiobox);
//
//	juce::Rectangle<int> attackbox(getLocalBounds());
//	attackbox.setWidth(getWidth() / 6);
//	attackbox.setHeight(getHeight() / 4);
//	attackbox.translate(220, 300);
////	attackSlider->setBounds(attackbox);
//
//	juce::Rectangle<int> releasebox(getLocalBounds());
//	releasebox.setWidth(getWidth() / 6);
//	releasebox.setHeight(getHeight() / 4);
//	releasebox.translate(320, 300);
////	releaseSlider->setBounds(releasebox);
//
//	juce::Rectangle<int> gainbox(getLocalBounds());
//	gainbox.setWidth(getWidth() / 6);
//	gainbox.setHeight(getHeight() / 4);
//	gainbox.translate(420, 300);
////	gainSlider->setBounds(gainbox);

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