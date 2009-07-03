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
			class ScriptState;
			
			class ComponentScript {
			public:
				ComponentScript(const std::string & name, TGen::Engine::Script::EntityScript * entityScript);
				virtual ~ComponentScript();
				
				void registerFunction(const std::string & name, int (*func) (lua_State *L));
				void pushComponent(TGen::Engine::Script::ScriptState & scriptState);
				void pushEntity(TGen::Engine::Script::ScriptState & scriptState);
				const std::string & getName() const;
				virtual void setEnabled(bool enabled) {}
				virtual bool getEnabled() const {return true; }
				
				TGen::Engine::Script::ScriptState & getScriptState() const;
				
			private:
				static int luaOwner(lua_State * vm);
				static int luaName(lua_State * vm);
				static int luaEnable(lua_State * vm);
				static int luaDisable(lua_State * vm);
				static int luaIsEnabled(lua_State * vm);
				
				TGen::Engine::Script::Subsystem & creator;
				TGen::Engine::Script::EntityScript * entityScript;
				std::string name;
				int beginTop;
			};
		}
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_COMPONENTSCRIPT_H

