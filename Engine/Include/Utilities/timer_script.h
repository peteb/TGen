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

#include "triggerable.h"
#include "symbols.h"
#include "componentlink.h"

namespace TGen {
	namespace Engine {
		class ComponentLinker;
		class TriggerContext;
		
		namespace Utilities {
			class Timer;
			
			class TimerScript {
			public:	
				TimerScript(Timer & timer);
				
				void link(const TGen::Engine::ComponentLinker & linker);
				bool trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode);		
				void setEventTick(const std::string & eventName);
				void tick();
				
			private:
				Timer & timer;
				TGen::Engine::EventDelegate eventTick;
				
				static TGen::Engine::Symbol symbolReset, symbolSetInterval, symbolGetInterval, symbolGetElapsedTime;
			};
			
		} // !Utilities
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_UTILITIES_TIMER_SCRIPT_H

