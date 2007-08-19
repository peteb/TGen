/*
 *  sphere.cpp
 *  TGen Math
 *
 *  Created by Peter Backman on 7/20/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "sphere.h"
#include <cmath>

TGen::Sphere::Sphere()
	: radius(0.0f)
{
}

TGen::Sphere::Sphere(const TGen::Vector3 & center, scalar radius)
	: center(center)
	, radius(radius) 
{
}


void TGen::Sphere::calculate(const TGen::Vector3 & min, const TGen::Vector3 & max) {
	scalar width = fabs(max.x - min.x);
	scalar height = fabs(max.y - min.y);
	scalar depth = fabs(max.z - min.z);
	
	radius = std::max(min.getMagnitude(), max.getMagnitude());
	center = TGen::Vector3(min.x + width / 2.0, min.y + height / 2.0, min.z + depth / 2.0);
}

TGen::Sphere::Sphere(const TGen::Vector3 & min, const TGen::Vector3 & max) {
	calculate(min, max);
}
