/*
 *  angle.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/25/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "angle.h"

const scalar TGen::Radian::RoundAngle = scalar(2.0 * TGen::PI);
const scalar TGen::Degree::RoundAngle = scalar(360.0);
const scalar TGen::UnitAngle::RoundAngle = scalar(1.0);

scalar TGen::Sine(const TGen::Radian & radians) {
	return scalar(sin(radians.angle));
}

scalar TGen::Cosine(const TGen::Radian & radians) {
	return scalar(cos(radians.angle));
}