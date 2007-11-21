/*
 *  light.cpp
 *  TGen Graphics
 *
 *  Created by Peter Backman on 11/18/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "light.h"

TGen::Light::Light()
	: constantAttenuation(0.0)
	, linearAttenuation(0.0)
	, quadraticAttenuation(0.0)
	, spotCutoff(0.0)
	, spotExponent(0.0)
{
}

