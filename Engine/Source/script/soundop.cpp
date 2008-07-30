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
#include "componentlinker.h"

TGen::Engine::Script::SoundOperation::SoundOperation() 
	: source(NULL)
	, sound(NULL)
	, TGen::Engine::Script::EventOperation(NULL)
{
	
}

void TGen::Engine::Script::SoundOperation::trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode) {
	if (source && sound) {
		source->addChannel(sound->spawnChannel(false));
	}
}

void TGen::Engine::Script::SoundOperation::link(const TGen::Engine::ComponentLinker & linker) {
	if (!sourceName.empty()) {
		source = dynamic_cast<TGen::Engine::Sound::Source *>(linker.getComponent(sourceName));
		if (!source)
			throw TGen::RuntimeException("Script::SoundOperation::link", "failed to get source " + sourceName);
	
		sound = source->getCreator().getSound(soundName);
	}
	
}

void TGen::Engine::Script::SoundOperation::setSource(const std::string & sourceName) {
	this->sourceName = sourceName;
}

void TGen::Engine::Script::SoundOperation::setSound(const std::string & soundName) {
	this->soundName = soundName;
}

