/*
 *  soundop.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/12/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SOUNDOP_H
#define _TGEN_ENGINE_SOUNDOP_H

#include "eventoperation.h"

namespace TGen {
	namespace Engine {
		namespace Sound {
			class Sound;
			class Source;
		}
		
		namespace Script {
			class SoundOperation : public TGen::Engine::Script::EventOperation {
			public:
				SoundOperation();
				
				void trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode);
				void linkGlobally(TGen::Engine::EntityList & entities, TGen::Engine::Entity & entity);
				
				void setSource(const std::string & sourceName);
				void setSound(const std::string & soundName);
				
			private:
				std::string sourceName, soundName;
				TGen::Engine::Sound::Source * source;
				TGen::Engine::Sound::Sound * sound;
			};
			
		} // !Script
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SOUNDOP_H

