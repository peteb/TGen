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
#include "script/componentscript.h"

struct lua_State;

namespace TGen {
	namespace Engine {
		class ComponentLinker;
		
		namespace Script {
			class EntityScript;
		}
		
		namespace Utilities {
			class Timer;
			
			class TimerScript : public TGen::Engine::Script::ComponentScript {
			public:	
				TimerScript(const std::string & name, Timer & timer, TGen::Engine::Script::EntityScript * entityScript);
				
				void tick();
				
			private:
				static int luaInterval(lua_State * vm);
				static int luaSetInterval(lua_State * vm);
				static int luaEnable(lua_State * vm);
				static int luaDisable(lua_State * vm);
				
				Timer & timer;
				TGen::Engine::Script::EntityScript * scriptEntity;
			};
			
		} // !Utilities
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_UTILITIES_TIMER_SCRIPT_H

