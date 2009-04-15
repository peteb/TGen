/*
 *  entityscript.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 2/17/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SCRIPT_ENTITYSCRIPT_H
#define _TGEN_ENGINE_SCRIPT_ENTITYSCRIPT_H

#include <string>

class lua_State;

namespace TGen {
	namespace Engine {
		class WorldObject;
		class Entity;
		
		namespace Script {
			class Subsystem;
			class ComponentScript;
			class ScriptState;
			
			class EntityScript {
			public:
				~EntityScript();

				TGen::Engine::Script::Subsystem & getCreator();		
				void onCreation();
				std::string getName() const;
				
				void pushEntity(TGen::Engine::Script::ScriptState & scriptState);
				void registerWorldInterface(TGen::Engine::WorldObject * worldInterface);
				
				friend class TGen::Engine::Script::Subsystem;
				
			private:
				EntityScript(TGen::Engine::Entity & entity, TGen::Engine::Script::Subsystem & creator);

				static int luaGetName(lua_State * vm);
				//static int luaGetWorldInterface(lua_State * vm);
				
				TGen::Engine::Script::Subsystem & creator;
				TGen::Engine::Entity & entity;
			};
		} // !Script
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCRIPT_ENTITYSCRIPT_H

