/*
 *  timer.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/24/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_UTILITIES_TIMER_H
#define _TGEN_ENGINE_UTILITIES_TIMER_H

#include "component.h"
#include "componentlink.h"

namespace TGen {
	namespace Engine {
		namespace Utilities {
			class Timer : public TGen::Engine::Component {
			public:
				Timer(const std::string & name);
				
				void link(const TGen::Engine::ComponentLinker & linker);
				void trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode);
				void update(scalar dt);
				void reset();
				
				void setEnabled(bool enabled);
				void setEventTick(const std::string & eventName);
				void setInterval(scalar interval);
				
			private:
				void tick();
				
				TGen::Engine::EventDelegate eventTick;
				scalar interval, totalTime, accumTime;
				bool enabled;
				
				static TGen::Engine::Symbol symbolReset;
			};
		} // !Utilities
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_UTILITIES_TIMER_H

