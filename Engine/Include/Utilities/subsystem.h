/*
 *  subsystem.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/24/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_UTILITIES_SUBSYSTEM_H
#define _TGEN_ENGINE_UTILITIES_SUBSYSTEM_H

#include "../subsystem.h"

namespace TGen {
	namespace Engine {
		namespace Utilities {
			class Timer;
			
			class Subsystem : public TGen::Engine::Subsystem {
			public:
				Subsystem();
				~Subsystem();
				
				TGen::Engine::Component * createComponent(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties);
				void update(scalar dt);
				
			private:
				std::vector<Timer *> timers;
			};
			
		} // !Utilities
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SUBSYSTEM_H

