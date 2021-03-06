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
#include "script/scriptstate.h"
#include "script/envscript.h"

struct lua_State;

namespace TGen {
	namespace Engine {
		class StandardLogs;
		class Filesystem;
		class Entity;
		
		namespace Script {
			class EntityScript;
			
			class Subsystem : public TGen::Engine::Subsystem {
			public:
				Subsystem(TGen::Engine::StandardLogs & logs, TGen::Engine::Filesystem & filesystem, const std::string & mapname);
				~Subsystem();
				
				TGen::Engine::Component * createComponent(const std::string & name, TGen::Engine::Entity & entity, const TGen::PropertyTree & properties);
				TGen::Engine::ComponentRecipe * createComponentRecipe(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties);
				TGen::Engine::Script::EntityScript * createScriptEntity(TGen::Engine::Entity & entity);
				
				ScriptState & getScriptState();
				
				void executeScripts(const std::string & path, bool recurse = false);
				void update(scalar dt);
				
			private: 
				static int luaWOWorldPosition(lua_State * vm);
				static int luaWOSetWorldPosition(lua_State * vm);
				static int luaWOWorldOrientation(lua_State * vm);
				static int luaWOWorldVelocity(lua_State * vm);
				static int luaParseProperties(lua_State * vm);
				
				static void addPropertyNode(const TGen::PropertyTree & node, TGen::Engine::Script::ScriptState & scriptState);
				
				void loadScriptFile(const std::string & filename);
				
				TGen::Engine::StandardLogs & logs;
				TGen::Engine::Filesystem & filesystem;
				TGen::Engine::Script::ComponentFactory componentFactory;
				
				TGen::Engine::Script::ScriptState scriptState;
				TGen::Engine::Script::EnvironmentScript envScript;
				int lastStackTop;
				float timeSinceGC;
			};
			
		} // !Script
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCRIPT_SUBSYSTEM_H

