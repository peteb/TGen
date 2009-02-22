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
		namespace Script {
			class Subsystem;
			class ComponentScript;
			
			class EntityScript {
			public:
				~EntityScript();

				TGen::Engine::Script::Subsystem & getCreator();
				TGen::Engine::Script::ComponentScript * createScriptComponent(const std::string & name, void * self);
		
				void onCreation();
				
				std::string getName() const;

				
				static int luaGetName(lua_State * vm);

				friend class TGen::Engine::Script::Subsystem;
				
			private:
				EntityScript(const std::string & name, TGen::Engine::Script::Subsystem & creator);

				TGen::Engine::Script::Subsystem & creator;
				std::string name;
			};
		} // !Script
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCRIPT_ENTITYSCRIPT_H

