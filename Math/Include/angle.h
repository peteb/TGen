/*
 *  angle.h
 *  tgen
 *
 *  Created by Peter Backman on 6/25/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ANGLE_H
#define _TGEN_ANGLE_H

#include "tgen_core.h"
#include <cmath>

namespace TGen {
	const scalar PI = scalar(3.1415926536);
	
	class Angle {
	public:
		Angle(scalar anAngle = 0.0) : angle(anAngle) {}
		Angle(const Angle & anAngle) {setAngle(anAngle); }
		virtual ~Angle() {}
		
		virtual scalar getRoundAngle() const abstract;
		void setAngle(const Angle & anAngle) {angle = (anAngle.angle / anAngle.getRoundAngle()) * getRoundAngle(); }
		bool isAcute(const Angle & anAngle) const {return (fabs((anAngle.angle / anAngle.getRoundAngle()) * getRoundAngle() - angle) < getRoundAngle() * 0.25f); }
		
		virtual Angle & operator =(float anAngle) {angle = anAngle; return *this; }
				
		scalar angle;
	};
	
	class Radian;
	
	class Degree : public Angle {
	public:
		Degree(scalar angle = 0.0) : Angle(angle) {}
		Degree(const Angle & anAngle) {setAngle(anAngle); }
		
		inline scalar getRoundAngle() const {return RoundAngle; }
		
		Degree & operator =(const Angle & anAngle) {angle = Degree(anAngle).angle; return *this; }
		inline Degree operator +(Angle & anAngle) {Degree ret(angle + Degree(anAngle).angle); return ret; }		// OPT: will probably be called pretty often, so, something faster than "Degree(anAngle).angle" should be used
		inline Degree operator -(Angle & anAngle) {Degree ret(angle - Degree(anAngle).angle); return ret; }
		inline Angle & operator +=(Angle & anAngle) {*this = *this + anAngle; return *this; }
		inline Angle & operator -=(Angle & anAngle) {*this = *this - anAngle; return *this; }
		
		const static scalar RoundAngle;
	};
	
	class Radian : public Angle {
	public:
		Radian(scalar angle = 0.0) : Angle(angle) {}
		Radian(const Angle & anAngle) {setAngle(anAngle); }
		
		inline scalar getRoundAngle() const {return RoundAngle; }
		
		Radian & operator =(const Degree & anAngle) {angle = Radian(anAngle).angle; return *this; }			
		inline Radian operator +(Angle & anAngle) {Radian ret(angle + Radian(anAngle).angle); return ret; }
		inline Radian operator -(Angle & anAngle) {Radian ret(angle - Radian(anAngle).angle); return ret; }
		inline Angle & operator +=(Angle & anAngle) {*this = *this + anAngle; return *this; }
		inline Angle & operator -=(Angle & anAngle) {*this = *this - anAngle; return *this; }
		
		const static scalar RoundAngle;	
	};
	
	class UnitAngle : public Angle {		// 1.0 = 360 = 2PI = ett helt varv
	public:
		UnitAngle(scalar angle = 0.0) : Angle(angle) {}
		UnitAngle(const Angle & anAngle) {setAngle(anAngle); }
		
		inline scalar getRoundAngle() const {return RoundAngle; }
		
		UnitAngle & operator =(const Degree & anAngle) {angle = UnitAngle(anAngle).angle; return *this; }
		inline UnitAngle operator +(Angle & anAngle) {UnitAngle ret(angle + UnitAngle(anAngle).angle); return ret; }
		inline UnitAngle operator -(Angle & anAngle) {UnitAngle ret(angle - UnitAngle(anAngle).angle); return ret; }
		inline Angle & operator +=(Angle & anAngle) {*this = *this + anAngle; return *this; }
		inline Angle & operator -=(Angle & anAngle) {*this = *this - anAngle; return *this; }
		
		const static scalar RoundAngle;
	};
	
	scalar Sine(const Radian & radians);
	scalar Cosine(const Radian & radians);
} // !TGen

#endif // !_TGEN_ANGLE_H
