/*
 *  angle.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/25/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "angle.h"

using TGen::scalar;

const scalar TGen::Radian::RoundAngle = scalar(2.0 * TGen::PI);
const scalar TGen::Degree::RoundAngle = scalar(360.0);
const scalar TGen::UnitAngle::RoundAngle = scalar(1.0);

scalar TGen::Sin(const TGen::Angle & angle) {
	return scalar(sin(TGen::Radian(angle).angle));
}

scalar TGen::Cos(const TGen::Angle & angle) {
	return scalar(cos(TGen::Radian(angle).angle));
}

// TODO: fixa glowmap