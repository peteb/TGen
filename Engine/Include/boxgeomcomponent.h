/*
 *  boxgeomcomponent.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/2/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_BOXGEOMCOMPONENT_H
#define _TGEN_ENGINE_BOXGEOMCOMPONENT_H

#include "physics/geom.h"

namespace TGen {
	class Vector3;
	
	namespace Engine {
		class Entity;
		class EntityList;
		
		namespace Physics {
			class BoxGeom : public TGen::Engine::Physics::Geom {
			public:
				BoxGeom(const std::string & name, const TGen::Vector3 & size, dSpaceID space);
			};
		} // !Physics
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_BOXGEOMCOMPONENT_H

