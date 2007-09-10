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

namespace TGen {
	bool isMathDebug();
	
	template<typename T>
	void Clamp(T & value, const T & low, const T & high) {
		if (value < low)
			value = low;
		else if (value > high)
			value = high;
	}
}

#endif // !_TGEN_MATH_H
