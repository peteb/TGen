/*
 *  timer_script.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/27/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_UTILITIES_TIMER_SCRIPT_H
#define _TGEN_ENGINE_UTILITIES_TIMER_SCRIPT_H

#include <string>

struct lua_State;

namespace TGen {
	namespace Engine {
		class ComponentLinker;
		
		namespace Script {
			class EntityScript;
			class ComponentScript;
		}
		
		namespace Utilities {
			class Timer;
			
			class TimerScript {
			public:	
				TimerScript(const std::string & name, Timer & timer, TGen::Engine::Script::EntityScript * entityScript);
				
				void tick();
				
			private:
				static int luaName(lua_State * vm);
				static int luaInterval(lua_State * vm);
				static int luaSetInterval(lua_State * vm);
				static int luaEnable(lua_State * vm);
				static int luaDisable(lua_State * vm);
				
				Timer & timer;
				TGen::Engine::Script::ComponentScript * scriptComponent;
				TGen::Engine::Script::EntityScript * scriptEntity;
				std::string name;
			};
			
		} // !Utilities
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_UTILITIES_TIMER_SCRIPT_H

