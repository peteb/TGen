/*
 *  resourcemanagerscript.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/11/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_RESOURCEMANAGERSCRIPT_H
#define _TGEN_ENGINE_RESOURCEMANAGERSCRIPT_H

struct lua_State;
#include <string>

namespace TGen {
	namespace Engine {
		class ResourceManager;
		
		namespace Script {
			class Subsystem;
		}
		
		class ResourceManagerScript {
		public:	
			ResourceManagerScript(TGen::Engine::ResourceManager & resources, TGen::Engine::Script::Subsystem & scriptSubsystem);
			
		private:
			static int luaMaterial(lua_State * vm);
			static int luaSound(lua_State * vm);

			static int luaMaterialName(lua_State * vm);
			
			void registerFunction(const std::string & name, int (*func) (lua_State *L));

			TGen::Engine::ResourceManager & resources;
			TGen::Engine::Script::Subsystem & scriptSubsystem;
			
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_RESOURCEMANAGERSCRIPT_H

