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

using TGen::scalar;

TGen::WaveGenerator::WaveGenerator(TGen::WaveType type, scalar base, scalar amplitude, scalar phase, scalar frequency) 
	: type(type)
	, base(base)
	, amplitude(amplitude)
	, phase(phase)
	, frequency(frequency)
	, startedAt(0.0) 
{
	phase = phase * frequency;
}

TGen::Color TGen::WaveGenerator::getColor(double time, float alpha) const {
	scalar value = getValue(time);

	return TGen::Color(value, value, value, alpha);
}

scalar TGen::WaveGenerator::getValue(double time) const {
	scalar x = (phase) - time * frequency;
	x -= floor(x);
	
	scalar y = 0.0f;
	
	switch (type) {
		case TGen::WaveSine:
			y = sin(x * TGen::TWO_PI);
			break;
			
		case TGen::WaveTriangle:
			x < 0.5f ? y = 4.0f * x - 1.0f : y = -4.0f * x + 3.0f;
			break;
			
		case TGen::WaveSquare:
			x < 0.5f ? y = -1.0f : y = 1.0f;
			break;
			
		case TGen::WaveSawtooth:
			y = 1.0f - x;
			break;
			
		case TGen::WaveInverseSawtooth:
			y = x;
			break;
			
		case TGen::WaveConst:
			y = time * frequency;
			break;
	}
	
	return base + y * amplitude;
}

TGen::WaveGenerator * TGen::WaveGenerator::clone() {
	return new TGen::WaveGenerator(*this);
}

