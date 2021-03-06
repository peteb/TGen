/*
 *  bodyscript.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/28/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_PHYSICS_BODYSCRIPT_H
#define _TGEN_ENGINE_PHYSICS_BODYSCRIPT_H

#include <string>
#include "script/componentscript.h"

struct lua_State;

namespace TGen {
	namespace Engine {
		namespace Script {
			class EntityScript;
		}
		
		namespace Physics {
			class Body;
			class EntityScript;
			
			class BodyScript : public TGen::Engine::Script::ComponentScript {
			public:
				BodyScript(const std::string & name, TGen::Engine::Physics::Body * body, TGen::Engine::Script::EntityScript * entityScript);
				~BodyScript();
				
				void setEnabled(bool enabled);
				bool getEnabled() const;
				
			private:
				static int luaResetForces(lua_State * vm);
				static int luaAddForce(lua_State * vm);
				
				TGen::Engine::Script::EntityScript * entityScript;
				TGen::Engine::Physics::Body * body;
			};
			
			
		} // !Physics
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_BODYSCRIPT_H

