/*
 *  subsystem.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/26/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SOUND_SUBSYSTEM_H
#define _TGEN_ENGINE_SOUND_SUBSYSTEM_H

#include "../subsystem.h"
#include "fmod/fmod.hpp"

namespace TGen {
	namespace Engine {
		class Filesystem;
		class StandardLogs;
		
		namespace Sound {
			class SoundSource;
			
			class Subsystem : public TGen::Engine::Subsystem {
			public:	
				Subsystem(TGen::Engine::StandardLogs & logs, TGen::Engine::Filesystem & fs);
				~Subsystem();
				
				TGen::Engine::Component * createComponent(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties);
				
				void update(scalar delta);
				
				
				static FMOD_RESULT F_CALLBACK openCallback(const char * name, int unicode, unsigned int * filesize, void ** handle, void ** userdata);
				static FMOD_RESULT F_CALLBACK closeCallback(void * handle, void * data);
				static FMOD_RESULT F_CALLBACK readCallback(void * handle, void * buffer, unsigned int sizebytes, unsigned int * bytesread, void * userdata);
				static FMOD_RESULT F_CALLBACK seekCallback(void * handle, unsigned int pos, void * userdata);
				static TGen::Engine::Filesystem * filesystem;
				
			private:
				typedef std::vector<TGen::Engine::Sound::SoundSource *> SoundSourceList;
				
				SoundSourceList soundSources;
				
				TGen::Engine::StandardLogs & logs;
				TGen::Engine::Filesystem & fs;
				FMOD::System * fmodSystem;
			};
			
		} // !Sound
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SUBSYSTEM_H

