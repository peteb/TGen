/*
 *  soundop.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/12/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "script/soundop.h"
#include "sound/sound.h"
#include "sound/soundsource.h"
#include "entitylist.h"
#include "entity.h"
#include "sound/subsystem.h"

TGen::Engine::Script::SoundOperation::SoundOperation() 
	: source(NULL)
	, sound(NULL)
{
	
}

void TGen::Engine::Script::SoundOperation::trigger(void ** argv, int argc) {
	if (source && sound) {
		source->addChannel(sound->spawnChannel(false));
	}
}

void TGen::Engine::Script::SoundOperation::linkGlobally(TGen::Engine::EntityList & entities, TGen::Engine::Entity & entity) {
	if (!sourceName.empty()) {
		source = dynamic_cast<TGen::Engine::Sound::Source *>(entities.getComponent(sourceName, entity, std::nothrow));
		if (!source)
			throw TGen::RuntimeException("Script::SoundOperation::linkGlobally", "failed to get source " + sourceName);
	
		sound = source->getCreator().getSound(soundName);
	}
	
}

void TGen::Engine::Script::SoundOperation::setSource(const std::string & sourceName) {
	this->sourceName = sourceName;
}

void TGen::Engine::Script::SoundOperation::setSound(const std::string & soundName) {
	this->soundName = soundName;
}

