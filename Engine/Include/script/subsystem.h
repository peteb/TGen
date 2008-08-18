/*
 *  subsystem.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/12/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SCRIPT_SUBSYSTEM_H
#define _TGEN_ENGINE_SCRIPT_SUBSYSTEM_H

#include "../subsystem.h"
#include "componentfactory.h"

struct lua_State;

namespace TGen {
	namespace Engine {
		class StandardLogs;
		class Filesystem;
		
		namespace Script {
			const char * LuaChunkReader(lua_State * vm, void * data, size_t * size);
			
			class Subsystem : public TGen::Engine::Subsystem {
			public:
				Subsystem(TGen::Engine::StandardLogs & logs, TGen::Engine::Filesystem & filesystem, const std::string & mapname);
				~Subsystem();
				
				TGen::Engine::Script::Event * createComponent(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties);
				TGen::Engine::ComponentRecipe * createComponentRecipe(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties);
				lua_State * getLuaContext() const;
				
			private: 
				void initializeLua(const std::string & mapname);
				void loadScripts(const std::string & path);
				void loadScriptFile(const std::string & filename);
				
				TGen::Engine::StandardLogs & logs;
				TGen::Engine::Filesystem & filesystem;
				TGen::Engine::Script::ComponentFactory componentFactory;
				
				lua_State * vm;
			};
			
		} // !Script
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCRIPT_SUBSYSTEM_H

