/*
 *  light.h
 *  TGen Graphics
 *
 *  Created by Peter Backman on 11/18/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_LIGHT_H
#define _TGEN_LIGHT_H

#include <tgen_core.h>
#include <tgen_math.h>
#include "color.h"

namespace TGen {
	class Light {
	public:
		Light();
		
		scalar calculateAttenuationDistance(scalar threshold, scalar maxdist);
		
		
		scalar constantAttenuation, linearAttenuation, quadraticAttenuation, spotCutoff, spotExponent;
		TGen::Vector3 spotDirection;
		TGen::Vector4 position;
		TGen::Color diffuse, specular;
	};
	
} // !TGen

#endif // !_TGEN_LIGHT_H
