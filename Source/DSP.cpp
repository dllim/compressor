/*
  ==============================================================================

    DSP.cpp
    Created: 15 Jan 2019 5:23:52pm
    Author:  daryl

  ==============================================================================
*/

#include "DSP.h"

Ballistics::Ballistics()
{
	prmAttack.store(100.f);
	prmRelease.store(100.f);

	isActive = false;
}

Ballistics::~Ballistics()
{

}

void Ballistics::setAttackTime(float attackTimeMs)
{
	prmAttack.store(jlimit(0.001f, 10000.f, attackTimeMs));
	mustUpdateProcessing = true;
}

void Ballistics::setReleaseTime(float releaseTimeMs)
{
	prmRelease.store(jlimit(10.f, 100000.f, releaseTimeMs));
	mustUpdateProcessing = true;
}

void Ballistics::initProcessing(double sR)
{
	sampleRate = sR;
	updateProcessing();
	reset();

	isActive = true;
}

void Ballistics::reset(float zeroValue)
{
	yold = zeroValue;
}

float Ballistics::processSingleSample(float &sample)
{
	if (mustUpdateProcessing)
		updateProcessing();

	auto timeCte = (sample > yold ? cteAT : cteRL);
	auto result = sample + timeCte * (yold - sample);

	yold = result;
	return result;
}

void Ballistics::updateProcessing()
{
	mustUpdateProcessing = false;
	auto Ts = 1.f / (float)sampleRate;

	cteAT = std::exp(-2.f * float_Pi * Ts * 1000.f / prmAttack.load());
	cteRL = std::exp(-2.f * float_Pi * Ts * 1000.f / prmRelease.load());
}