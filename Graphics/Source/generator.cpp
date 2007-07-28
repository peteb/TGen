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

TGen::WaveGenerator::WaveGenerator(scalar base, scalar amplitude, scalar phase, scalar frequency) 
	: base(base)
	, amplitude(amplitude)
	, phase(phase)
	, frequency(frequency)
	, startedAt(0.0) 
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
/*	scalar t = time - startedAt;
	return TGen::Sin(TGen::UnitAngle(frequency * (t - phase))) * amplitude + base;*/
	
	
	
	scalar x = scalar(time - startedAt - phase) * frequency;
	scalar y = 0.0f;
	
	x -= floorf(x);
	
	y = sinf(x * TGen::PI * 2.0);
	
	return y * amplitude + base;
	
	
	
//	return TGen::Sin(TGen::UnitAngle(((time - startedAt) + phase) * frequency)) * amplitude + base;
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
/*
 float Wave::Evaluate(float time) const
 {
	 // Evaluate y = offset + amplitude * f((time + phase) * frequency)
	 float x = (time + m_phase) * m_frequency;
	 x -= floorf(x); // Normalize
	 
	 float y;	
	 switch (m_type)
	 {
		 case SIN:
			 y = sinf(x * Math::TWO_PI);
			 break;
		 case TRIANGLE:
			 x < 0.5f ? y = 4.0f * x - 1.0f : y = -4.0f * x + 3.0f;
			 break;
		 case SQUARE:
			 x < 0.5f ? y = 1.0f : y = -1.0f;
			 break;
		 case SAWTOOTH:
			 y = x;
			 break;
		 case INVSAWTOOTH:
			 y = 1.0f - x;
			 break;
		 case NOISE:
			 y = (float)rand() / (float)RAND_MAX;
			 break;
	 }
	 
	 return y * m_amplitude + m_offset;
 }
 */

scalar TGen::SquareWaveGenerator::getValue(double time) const {
	/*scalar t = time - startedAt;
	scalar value = TGen::Sin(TGen::UnitAngle(frequency * (t - phase))) * amplitude + base;
	scalar fixed;
	
	if (value >= 0.0)
		fixed = base + amplitude;
	else
		fixed = base - amplitude;
	
	return fixed;*/
	
	scalar x = (time - startedAt - phase) * frequency;
	scalar y = 0.0f;

	x -= floorf(x);
	
	if (x < 0.5f)
		y = 1.0f;
	else
		y = -1.0f;
	
	return y * amplitude + base;
}

// TODO: flytta in det här i en klass... wave...

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
	scalar x = (time - startedAt - phase) * frequency;
	scalar y = 0.0f;
	
	x -= floorf(x);
	
	if (inverse)
		y = 1.0 - x;
	else
		y = x;
	
	return y * amplitude + base;
}