/*
 *  quaternion4.h
 *  TGen Math
 *
 *  Created by Peter Backman on 7/30/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_QUATERNION4_H
#define _TGEN_QUATERNION4_H

#include "types.h"
#include "matrix4x4.h"
#include "vector3.h"
#include "vector4.h"

namespace TGen {
	class Angle;
	
	class Quaternion4 {
	public:	
		Quaternion4();
		Quaternion4(scalar x, scalar y, scalar z, scalar w = 0.0);
		Quaternion4(const TGen::Vector3 & vector);
		
		scalar getMagnitude() const;
		Quaternion4 getNormalized() const;
		Quaternion4 & normalize();
		
		Quaternion4 & operator *= (const TGen::Quaternion4 & quat);
		Quaternion4 operator * (const TGen::Quaternion4 & quat) const;
		Quaternion4 operator * (scalar value) const;
		Quaternion4 operator + (const Quaternion4 & quat) const;
		Quaternion4 operator - (const Quaternion4 & quat) const;
		
		Quaternion4 operator - () const;
		operator TGen::Matrix4x4 () const;
		operator TGen::Vector3 () const;
		operator TGen::Vector4 () const;
		
		static Quaternion4 Rotation(const TGen::Vector3 axis, const TGen::Angle & angle);
		static Quaternion4 Slerp(const Quaternion4 & q1, const Quaternion4 & q2, scalar t);
		static scalar DotProduct(const Quaternion4 & q1, const Quaternion4 & q2);
		
		scalar x, y, z, w;
	};
	
	
} // !TGen

#endif // !_TGEN_QUATERNION4_H