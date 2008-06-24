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
		class WorldObject;
		
		namespace Sound {
			class Subsystem;
			class Sound;
			class Channel;
			
			class Source : public TGen::Engine::Component {
			public:
				Source(const std::string & name, const std::string & filename);
				virtual ~Source();
				
				void link(TGen::Engine::Sound::Subsystem & linker);
				void unlink();
				virtual void update(scalar dt);
				
				void setAutoplay(bool autoplay);
				void setLoop(bool loop);
				void setPrototype(bool prototype);
				
				virtual TGen::Engine::Sound::Channel * spawnChannel(bool paused);
				
			protected:
				typedef std::vector<TGen::Engine::Sound::Channel *> ChannelList;
				
				TGen::Engine::Sound::Sound * linkedSound;
				ChannelList channels;
				bool prototype;
				
			private:
				std::string filename;
				bool autoplay, loop, threedee;
			};
			
		} // !Sound		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SOUNDSOURCE_H

