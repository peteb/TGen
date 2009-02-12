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

#include "componentlink.h"

namespace TGen {
	namespace Engine {
		class ComponentLinker;
		
		namespace Utilities {
			class Timer;
			
			class TimerScript {
			public:	
				TimerScript(Timer & timer);
				
				void link(const TGen::Engine::ComponentLinker & linker);
				void tick();
				
			private:
				Timer & timer;
			};
			
		} // !Utilities
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_UTILITIES_TIMER_SCRIPT_H

