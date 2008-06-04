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
		class ObjectInterface;
		
		namespace Sound {
			class Subsystem;
			
			class SoundSource : public TGen::Engine::Component {
			public:
				SoundSource(const std::string & name, FMOD::Sound * sound, TGen::Engine::Sound::Subsystem & subsystem, const std::string & linkWith);
				~SoundSource();
				
				void linkLocally(TGen::Engine::Entity & entity);
				void linkGlobally(TGen::Engine::EntityList & entities);		
				void update(scalar dt);
				void play();
				void setSingleChannel(bool single);
				
			private:
				void spawnChannel();
				
				FMOD::Sound * sound;
				TGen::Engine::Sound::Subsystem & subsystem;
				TGen::Engine::ObjectInterface * track;
				
				bool shouldPlay, isPlaying, singleChannel;
				std::string linkWith;
				
				std::vector<FMOD::Channel *> channels;
			};
			
			class Source : public TGen::Engine::Component {
			public:
				Source(const std::string & name, const std::string & filename, bool stream);
				virtual ~Source();
				
				void link(TGen::Engine::Sound::Subsystem & linker);
				void unlink();
				
			private:
				FMOD::Sound * linkedSound;
				std::string filename;
				bool stream;
			};
			
		} // !Sound		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SOUNDSOURCE_H

