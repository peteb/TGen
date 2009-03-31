/*
 *  geomscript.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/24/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_PHYSICS_GEOMSCRIPT_H
#define _TGEN_ENGINE_PHYSICS_GEOMSCRIPT_H

#include "component.h"
#include <ode/ode.h>
#include <tgen_core.h>
#include "componentlink.h"
#include "physics/bodydelegate.h"

namespace TGen {
	namespace Engine {
		namespace Physics {
			class Geom;
			class EntityScript;
			
			class GeomScript {
			public:
				GeomScript(const std::string & name, TGen::Engine::Physics::Geom * geom, TGen::Engine::Script::EntityScript * entityScript);
				~GeomScript();

				void onCollision(scalar force, TGen::Engine::Physics::Geom * with);
				
			private:
				TGen::Engine::Script::ComponentScript * scriptComponent;
				TGen::Engine::Script::EntityScript * entityScript;
				
				static int luaOwner(lua_State * vm);
				
				std::string name;
				TGen::Engine::Physics::Geom * geom;
			};
		
		} // !Physics
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_PHYSICS_GEOMSCRIPT_H
