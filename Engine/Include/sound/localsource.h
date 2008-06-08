/*
 *  localsource.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/30/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_LOCALSOURCE_H
#define _TGEN_ENGINE_LOCALSOURCE_H

#include "../component.h"
#include "sound/soundsource.h"

namespace TGen {
	namespace Engine {
		class Entity;
		
		namespace Sound {
			class LocalSource : public TGen::Engine::Sound::Source {
			public:
				LocalSource(const std::string & name, const std::string & filename, const std::string & linkWith);
				
				void linkLocally(TGen::Engine::Entity & entity);
				void update(scalar dt);
				void setMinMaxDistance(scalar min, scalar max);
				
				TGen::Engine::Sound::Channel * spawnChannel(bool paused);
				
			private:
				std::string linkWith;
				TGen::Engine::ObjectInterface * follow;
				
				scalar minDistance, maxDistance;
			};
			
		} // !Sound
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_LOCALSOURCE_H

