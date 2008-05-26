/*
 *  soundsource.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/26/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SOUNDSOURCE_H
#define _TGEN_ENGINE_SOUNDSOURCE_H

#include "../component.h"
#include "fmod/fmod.hpp"
#include <tgen_core.h>

namespace TGen {
	namespace Engine {
		namespace Sound {
			class SoundSource : public TGen::Engine::Component {
			public:
				SoundSource(const std::string & name, FMOD::Sound * sound);
				~SoundSource();
				
				void linkLocally(TGen::Engine::Entity & entity);
				void linkGlobally(TGen::Engine::EntityList & entities);		
				void update(scalar dt);
				
			private:
				FMOD::Sound * sound;
			};
		} // !Sound		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SOUNDSOURCE_H

