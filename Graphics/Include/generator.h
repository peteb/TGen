/*
 *  generator.h
 *  TGen Graphics
 *
 *  Created by Peter Backman on 7/8/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_GENERATOR_H
#define _TGEN_GENERATOR_H

#include "tgen_core.h"
#include "color.h"

namespace TGen {
	class ColorGenerator {
	public:
		virtual ~ColorGenerator() {}
		
		virtual TGen::Color getColor(double time, float alpha) const abstract;
	};
	
	class ScalarGenerator {
	public:
		virtual ~ScalarGenerator() {}
		
		virtual scalar getValue(double time) const abstract;
	};
	
	// rgbGen = new SineGenerator
	
	enum WaveType {
		WaveSine = 1,
		WaveSquare,
		WaveTriangle,
		WaveSawtooth,
		WaveInverseSawtooth,
		WaveConst,
	};
	
	class WaveGenerator : public ColorGenerator, public ScalarGenerator {
	public:
		WaveGenerator(WaveType type, scalar base, scalar amplitude, scalar phase, scalar frequency);
		~WaveGenerator() {}
		
		TGen::Color getColor(double time, float alpha) const;
		scalar getValue(double time) const;
		
	protected:
		WaveType type;
		scalar base, amplitude, phase, frequency;
		double startedAt;
	};
} // !TGen

#endif // !_TGEN_GENERATOR_H
