/*
 *  raygeom.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/27/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_PHYSICS_RAYGEOM_H
#define _TGEN_ENGINE_PHYSICS_RAYGEOM_H

#include "physics/geom.h"
#include <tgen_core.h>

namespace TGen {
	namespace Engine {
		namespace Physics {
			class RayGeom : public TGen::Engine::Physics::Geom {
			public:
				RayGeom(const std::string & name, scalar length, dSpaceID space);
				
			protected:
				void setOrientation(const TGen::Matrix3x3 & orientation);
			};
			
		} // !Physics
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_PHYSICS_RAYGEOM_H

