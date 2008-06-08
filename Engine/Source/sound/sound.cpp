/*
 *  sound.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/3/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "sound/sound.h"

#include <tgen_core.h>
#include "fmod/fmod_errors.h"

using TGen::Engine::Sound::Channel;

TGen::Engine::Sound::Sound::Sound(FMOD::Sound * sound) 
	: sound(sound)
{

}

TGen::Engine::Sound::Sound::~Sound() {
	if (!sound)
		return;
	
	FMOD_RESULT result = sound->release();
	
	if (result != FMOD_OK)
		throw TGen::RuntimeException("Sound::~Sound", "failed to release sound: ") << FMOD_ErrorString(result);
}

Channel * TGen::Engine::Sound::Sound::spawnChannel(bool paused) {
	FMOD::Channel * channel = NULL;
	FMOD::System * system = NULL;
	
	sound->getSystemObject(&system);
	FMOD_RESULT result = system->playSound(FMOD_CHANNEL_FREE, sound, paused, &channel);
	
	if (result != FMOD_OK)
		throw TGen::RuntimeException("Sound::Sound::spawnChannel", "failed to spawn channel: ") << FMOD_ErrorString(result);
	
	return new Channel(channel);
}
