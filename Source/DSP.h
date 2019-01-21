/*
  ==============================================================================

    DSP.h
    Created: 15 Jan 2019 5:24:03pm
    Author:  daryl

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode//JuceHeader.h"

class Ballistics
{
public:

	Ballistics();

	~Ballistics();

	void initProcessing(double sampleRate);

	void reset(float zeroValue = 0.f);

	float processSingleSample(float &sample);

	void setAttackTime(float attackTimeMs);

	void setReleaseTime(float releaseTimeMs);

private:
	void updateProcessing();

	double sampleRate;
	bool mustUpdateProcessing, isActive;

	std::atomic<float> prmAttack, prmRelease;

	float cteAT, cteRL;
	float yold;
};
