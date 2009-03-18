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
				Timer & timer;
				TGen::Engine::Script::ComponentScript * scriptComponent;
				std::string name;
			};
			
		} // !Utilities
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_UTILITIES_TIMER_SCRIPT_H

