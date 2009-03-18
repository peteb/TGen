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
#include <tgen_core.h>

namespace TGen {
	namespace Engine {
		namespace Utilities {
			class TimerScript;
			
			class Timer : public TGen::Engine::Component {
			public:
				Timer(const std::string & name);
				
				void link(const TGen::Engine::ComponentLinker & linker);
				void update(scalar dt);
				
				// TODO: wave timer
				
				void setScriptInterface(TGen::Engine::Utilities::TimerScript * scriptInterface);
				void reset();
				void setEnabled(bool enabled);
				void setInterval(scalar interval);
				void setOffsetTime(scalar time);
				scalar getInterval() const;
				scalar getElapsedTime() const;
				
			private:
				void tick();
				
				TGen::Engine::Utilities::TimerScript * scriptInterface;
				
				scalar interval, totalTime, accumTime, offsetTime;
				bool enabled;
			};
		} // !Utilities
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_UTILITIES_TIMER_H

