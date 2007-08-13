/*
 *  sphere.h
 *  TGen Math
 *
 *  Created by Peter Backman on 7/20/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_SPHERE_H
#define _TGEN_SPHERE_H

#include "vector3.h"

namespace TGen {
	class Sphere {
	public:
		Sphere();
		Sphere(const TGen::Vector3 & center, scalar radius);

		void calculate(const TGen::Vector3 & min, const TGen::Vector3 & max);
		
		TGen::Vector3 center;
		scalar radius;
	};
	
} // !TGen

#endif // !_TGEN_SPHERE_H
