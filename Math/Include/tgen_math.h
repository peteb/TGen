/*
 *  tgen_math.h
 *  TGen Math
 *
 *  Created by Peter Backman on 6/30/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_MATH_H
#define _TGEN_MATH_H

#include "angle.h"
#include "plane3.h"
#include "quaternion4.h"

#include "matrix3x3.h"
#include "matrix4x4.h"
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"

#include "rectangle.h"
#include "AABB.h"	// box.h?
#include "sphere.h"
#include "frustum.h"
#include "quad3.h"
#include "coordset3.h"
#include "coordset2.h"

namespace TGen {
	bool isMathDebug();
	
	typedef TGen::Matrix3x3 Rotation;
	
	template<typename T>
	T Clamp(T value, T low, T high) {
		if (value < low)
			return low;
		else if (value > high)
			return high;
		
		return value;
	}
	
	template<typename T>
	T IdentityClamp(T value) {
		if (value < 0.0)
			return 0.0;
		else if (value > 1.0)
			return 1.0;
		
		return value;
	}
	
	template<typename T>
	T Interpolate(const T & start, const T & end, float t) {
		return start + (end - start) * t;
	}
}

#endif // !_TGEN_MATH_H
