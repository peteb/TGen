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

#include "physics/geom.h"

namespace TGen {
	namespace Engine {
		namespace Physics {
			class SphereGeom : public TGen::Engine::Physics::Geom {
			public:
				SphereGeom(const std::string & name, float radius, dSpaceID space);
			};
		} // !Physics
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SPHEREGEOMCOMPONENT_H

