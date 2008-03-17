/*
 *  spheregeomcomponent.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/1/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SPHEREGEOMCOMPONENT_H
#define _TGEN_ENGINE_SPHEREGEOMCOMPONENT_H

#include "geomcomponent.h"

namespace TGen {
	namespace Engine {
		class Entity;
		class EntityList;
		
		class SphereGeomComponent : public TGen::Engine::GeomComponent {
		public:
			SphereGeomComponent(const std::string & name, float radius, dSpaceID space);
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SPHEREGEOMCOMPONENT_H

