/*
 *  componentscript.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 2/17/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_COMPONENTSCRIPT_H
#define _TGEN_ENGINE_COMPONENTSCRIPT_H

#include <string>

class lua_State;

namespace TGen {
	namespace Engine {
		namespace Script {
			class Subsystem;
			class EntityScript;
			class Subsystem;
			
			class ComponentScript {
			public:
				~ComponentScript();
				
				void registerFunction(const std::string & name, int (*func) (lua_State *L));
				
				friend class TGen::Engine::Script::EntityScript;
				
			private:
				ComponentScript(TGen::Engine::Script::EntityScript * entityScript, const std::string & name, TGen::Engine::Script::Subsystem & creator);
				
				TGen::Engine::Script::Subsystem & creator;
				TGen::Engine::Script::EntityScript * entityScript;
				std::string name;
			};
		}
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_COMPONENTSCRIPT_H

