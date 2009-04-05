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

struct lua_State;

namespace TGen {
	namespace Engine {
		namespace Script {
			class EntityScript;
			class ComponentScript;
		}
		
		namespace Physics {
			class Body;
			class EntityScript;
			
			class BodyScript {
			public:
				BodyScript(const std::string & name, TGen::Engine::Physics::Body * body, TGen::Engine::Script::EntityScript * entityScript);
				~BodyScript();
				
			private:
				TGen::Engine::Script::ComponentScript * scriptComponent;
				TGen::Engine::Script::EntityScript * entityScript;
				
				static int luaOwner(lua_State * vm);
				static int luaEnable(lua_State * vm);
				
				std::string name;
				TGen::Engine::Physics::Body * body;
			};
			
			
		} // !Physics
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_BODYSCRIPT_H

