/*
 *  generator.cpp
 *  TGen Graphics
 *
 *  Created by Peter Backman on 7/8/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "generator.h"
#include "tgen_math.h"

TGen::WaveGenerator::WaveGenerator(scalar base, scalar amplitude, scalar phase, scalar frequency) :
	base(base), amplitude(amplitude), phase(phase), frequency(frequency), startedAt(0.0) 
{

}


TGen::SineWaveGenerator::SineWaveGenerator(scalar base, scalar amplitude, scalar phase, scalar frequency) : 
	TGen::WaveGenerator(base, amplitude, phase, frequency) 
{

}

TGen::Color TGen::SineWaveGenerator::getColor(double time, float alpha) const {
	scalar value = getValue(time);
	//Lead::Math::Clamp(value, 0.0, 1.0);
	
	return TGen::Color(value, value, value, alpha);
}

scalar TGen::SineWaveGenerator::getValue(double time) const {	
	return TGen::Sine(TGen::UnitAngle(((time - startedAt) + phase) * frequency)) * amplitude + base;
}


TGen::SquareWaveGenerator::SquareWaveGenerator(scalar base, scalar amplitude, scalar phase, scalar frequency) :
	TGen::WaveGenerator(base, amplitude, phase, frequency) 
{

}

TGen::Color TGen::SquareWaveGenerator::getColor(double time, float alpha) const {
	scalar value = getValue(time);
	//Lead::Math::Clamp(value, 0.0, 1.0);
	
	return TGen::Color(value, value, value, alpha);	
}

scalar TGen::SquareWaveGenerator::getValue(double time) const {
	scalar value = TGen::Sine(TGen::UnitAngle(((time - startedAt) + phase) * frequency));
	scalar fixed;
	
	if (value >= 0.0)
		fixed = base + amplitude;
	else
		fixed = base - amplitude;
	
	return fixed;
}


TGen::SawtoothWaveGenerator::SawtoothWaveGenerator(scalar base, scalar amplitude, scalar phase, scalar frequency, bool inverse) 
	: TGen::WaveGenerator(base, amplitude, phase, frequency)
	, lastValue(0.0f) 
	, inverse(inverse)
{
}

TGen::Color TGen::SawtoothWaveGenerator::getColor(double time, float alpha) const {
	scalar value = getValue(time);
	 //Lead::Math::Clamp(value, 0.0, 1.0);
	
	return TGen::Color(value, value, value, alpha);	
}

scalar TGen::SawtoothWaveGenerator::getValue(double time) const {
	float a = 1.0 / frequency;
	time += phase * frequency;
	scalar value = 0.0f;
	
	if (!inverse)
		value = (2.0 * (time / a - floor(time / a + 0.5))) * amplitude + base;
	else
		value = -(2.0 * (time / a - floor(time / a + 0.5))) * amplitude + base;
	
	return value;
}