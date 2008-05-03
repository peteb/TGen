/*
 *  bipedalgeom.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/1/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_PHYSICS_BIPEDALGEOM_H
#define _TGEN_ENGINE_PHYSICS_BIPEDALGEOM_H

#include "physics/geom.h"
#include <tgen_core.h>

namespace TGen {
	namespace Engine {
		namespace Physics {
			class BipedalGeom : public TGen::Engine::Physics::Geom {
			public:
				BipedalGeom(const std::string & name, dSpaceID space, scalar capRadius, scalar length);
				
				void preStep();
				void postStep();
				
			private:
				scalar capRadius, length;
			};
		} // !Physics
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_PHYSICS_BIPEDALGEOM_H

