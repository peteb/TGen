/*
 *  sound.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/3/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SOUND_SOUND_H
#define _TGEN_ENGINE_SOUND_SOUND_H

#include "fmod/fmod.hpp"
#include "sound/channel.h"

namespace TGen {
	namespace Engine {
		namespace Sound {
			class Sound {
			public:	
				Sound(FMOD::Sound * sound, bool stream);
				virtual ~Sound();
				
				TGen::Engine::Sound::Channel * spawnChannel(bool paused);
				bool isStream() const;
				
			private:
				FMOD::Sound * sound;
				bool stream;
			};
			
		} // !Sound
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SOUND_SOUND_H

