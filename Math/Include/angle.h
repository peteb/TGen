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
	const scalar TWO_PI = scalar(6.28318530718);
	const scalar HALF_PI = scalar(1.57079632679);
	
	class Angle {
	protected:
		Angle(scalar anAngle = 0.0) : angle(anAngle) {}
		Angle(const Angle & anAngle) {setAngle(anAngle); }

	public:
		virtual ~Angle() {}
		
		virtual scalar getRoundAngle() const abstract;
		void setAngle(const Angle & angle) {this->angle = (angle.angle / angle.getRoundAngle()) * getRoundAngle(); }
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
		inline Degree operator +(Angle & anAngle) {Degree ret(angle + Degree(anAngle).angle); return ret; }
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
		inline Radian operator / (scalar value) {return Radian(angle / value); }
		
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
	
	scalar Sin(const Angle & angle);
	scalar Cos(const Angle & angle);
} // !TGen

#endif // !_TGEN_ANGLE_H
