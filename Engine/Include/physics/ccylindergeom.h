/*
 *  ccylindergeom.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/26/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_PHYSICS_CCYLINDERGEOM_H
#define _TGEN_ENGINE_PHYSICS_CCYLINDERGEOM_H

#include "physics/geom.h"
#include <tgen_core.h>

namespace TGen {
	namespace Engine {
		namespace Physics {
			class CCylinderGeom : public TGen::Engine::Physics::Geom {
			public:
				CCylinderGeom(const std::string & name, scalar radius, scalar length, dSpaceID space);
			};
			
		} // !Physics
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_PHYSICS_CCYLINDERGEOM_H

