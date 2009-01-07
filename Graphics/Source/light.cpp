/*
 *  light.cpp
 *  TGen Graphics
 *
 *  Created by Peter Backman on 11/18/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "light.h"

using TGen::scalar;

TGen::Light::Light()
	: constantAttenuation(0.0)
	, linearAttenuation(0.0)
	, quadraticAttenuation(0.0)
	, spotCutoff(0.0)
	, spotExponent(0.0)
{
}

scalar TGen::Light::calculateAttenuationDistance(scalar threshold, scalar maxdist) {
	scalar ret = 0.0f;
	
	for (scalar a = 0.0f; ; a += 1.0f) {	
		if ((1.0f / (constantAttenuation + linearAttenuation * a + quadraticAttenuation * a * a) < threshold) || a > maxdist) {
			ret = a;
			break;
		}
	}
	
	return ret;
}
