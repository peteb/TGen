/*
 *  planegeomcomponent.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/1/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_PLANEGEOMCOMPONENT_H
#define _TGEN_ENGINE_PLANEGEOMCOMPONENT_H

#include "physics/geom.h"

namespace TGen {
	class Plane3;
	
	namespace Engine {
		namespace Physics {
			class PlaneGeom : public TGen::Engine::Physics::Geom {
			public:
				PlaneGeom(const std::string & name, const TGen::Plane3 & plane, dSpaceID space);
			};
		} // !Physics
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_PLANEGECOMPONENT_H
