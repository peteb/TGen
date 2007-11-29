/*
 *  light.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 11/25/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_LIGHT_H
#define _TGEN_ENGINE_LIGHT_H

#include <tgen_math.h>
#include <tgen_graphics.h>

namespace TGen {
	namespace Engine {
		enum LightType {
			LightDirectional = 0,
			LightPositional = 1,
			LightSpotlight = 2,
		};
		
		class Light {
		public:
			// light type.... inte material :(
			// ska vara en data-component på entity?
			int type;			
			bool clipBoundingBox;
			TGen::AABB boundingBox;
			TGen::Light light;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_LIGHT_H
