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
			class SourceScript;
			
			class Source : public TGen::Engine::Component {
			public:
				Source(const std::string & name, const std::string & filename, TGen::Engine::Sound::Subsystem & creator);
				virtual ~Source();
				
				void linkSubsystem(TGen::Engine::Sound::Subsystem & subsystem);
				void unlink();
				virtual void update(scalar dt);
				
				void setAutoplay(bool autoplay);
				void setLoop(bool loop);
				void setPrototype(bool prototype);
				void addChannel(TGen::Engine::Sound::Channel * channel);
				void setVolume(float volume);
				void setScriptInterface(TGen::Engine::Sound::SourceScript * scriptInterface);
				void removeAllChannels();
				
				virtual void playSound(TGen::Engine::Sound::Sound * sound) abstract;
				
				TGen::Engine::Sound::Subsystem & getCreator() const;
				
				virtual TGen::Engine::Sound::Channel * spawnChannel(bool paused);
				
			protected:
				typedef std::vector<TGen::Engine::Sound::Channel *> ChannelList;
				
				TGen::Engine::Sound::Sound * linkedSound;
				TGen::Engine::Sound::SourceScript * scriptInterface;
				ChannelList channels;
				bool prototype;
				TGen::Engine::Sound::Subsystem & creator;
				
				float volume;
				
			private:
				std::string filename;
				bool autoplay, loop, threedee;
			};
			
		} // !Sound		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SOUNDSOURCE_H

