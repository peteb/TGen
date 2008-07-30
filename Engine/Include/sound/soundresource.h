/*
 *  soundresource.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/20/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SOUND_SOUNDRESOURCE_H
#define _TGEN_ENGINE_SOUND_SOUNDRESOURCE_H

#include "resourcecomponent.h"

namespace TGen {
	namespace Engine {		
		namespace Sound {
			class Subsystem;
			class Sound;
			
			class SoundResource : public TGen::Engine::ResourceComponent {
			public:
				SoundResource(const std::string & name, TGen::Engine::Sound::Subsystem & subsystem);
				
				void * getData() const;
				void link(const TGen::Engine::ComponentLinker & linker);
				void setSoundName(const std::string & name);
				
			private:
				TGen::Engine::Sound::Subsystem & subsystem;
				std::string resourceName;
				TGen::Engine::Sound::Sound * linkedSound;
			};
			
		} // !Sound
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SOUND_SOUNDRESOURCE_H

