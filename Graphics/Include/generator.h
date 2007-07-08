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
	
	class WaveGenerator : public ColorGenerator, public ScalarGenerator {
	public:
		WaveGenerator(scalar base, scalar amplitude, scalar phase, scalar frequency);
		virtual ~WaveGenerator() {}
		
		virtual TGen::Color getColor(double time, float alpha) const abstract;
		virtual scalar getValue(double time) const abstract;
		
	protected:
		scalar base, amplitude, phase, frequency;
		double startedAt;
	};
	
	class SineWaveGenerator : public WaveGenerator {
	public:	
		SineWaveGenerator(scalar base, scalar amplitude, scalar phase, scalar frequency);
		~SineWaveGenerator() {}
		
		TGen::Color getColor(double time, float alpha) const;
		scalar getValue(double time) const;			
	};
	
	class SquareWaveGenerator : public WaveGenerator {
	public:
		SquareWaveGenerator(scalar base, scalar amplitude, scalar phase, scalar frequency);
		
		TGen::Color getColor(double time, float alpha) const;
		scalar getValue(double time) const;
	};
	
	class SawtoothWaveGenerator : public WaveGenerator {
	public:
		SawtoothWaveGenerator(scalar base, scalar amplitude, scalar phase, scalar frequency);
		
		TGen::Color getColor(double time, float alpha) const;
		scalar getValue(double time) const;
		
	private:
		mutable scalar lastValue;
	};	
} // !TGen

#endif // !_TGEN_GENERATOR_H
