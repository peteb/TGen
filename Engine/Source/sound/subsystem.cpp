/*
 *  subsystem.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/26/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "sound/subsystem.h"
#include "log.h"
#include "fmod/fmod.hpp"
#include "fmod/fmod_errors.h"

TGen::Engine::Sound::Subsystem::Subsystem(TGen::Engine::StandardLogs & logs) 
	: logs(logs)
{
	logs.info["snd+"] << "*** INITIALIZING SOUND ***" << TGen::endl;
	
	FMOD_RESULT result;
	FMOD::System *system;
	
	result = FMOD::System_Create(&system);
	if (result != FMOD_OK)
		throw TGen::RuntimeException("Sound::Subsystem", "Failed to create fmod system: ") << FMOD_ErrorString(result) << " (" << result << ")";
	
	result = system->init(100, FMOD_INIT_NORMAL, 0);
	if (result != FMOD_OK)
		throw TGen::RuntimeException("Sound::Subsystem", "Failed to initialize fmod system: ") << FMOD_ErrorString(result) << " (" << result << ")";
	
	logs.info["snd+"] << "fmod initialized" << TGen::endl;
	
}

TGen::Engine::Sound::Subsystem::~Subsystem() {
	logs.info["snd-"] << "*** SHUTTING DOWN SOUND ***" << TGen::endl;
}


TGen::Engine::Component * TGen::Engine::Sound::Subsystem::createComponent(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties) {
	throw TGen::RuntimeException("blah", "blah");
	
	return NULL;
}

