/*
 *  localsource.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/30/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "sound/localsource.h"
#include "sound/channel.h"
#include "sound/sound.h"

#include "entity.h"
#include "componentinterfaces.h"
#include "componentlinker.h"

TGen::Engine::Sound::LocalSource::LocalSource(const std::string & name, const std::string & filename, const std::string & linkWith, TGen::Engine::Sound::Subsystem & creator)
	: TGen::Engine::Sound::Source(name, filename, creator)
	, linkWith(linkWith)
	, linkedTo(NULL)
	, minDistance(1.0)
	, maxDistance(10000.0)
{
	
}

void TGen::Engine::Sound::LocalSource::link(const TGen::Engine::ComponentLinker & linker) {
	linkedTo = dynamic_cast<TGen::Engine::WorldObject *>(linker.getComponent(linkWith));
}

void TGen::Engine::Sound::LocalSource::update(scalar dt) {
	if (prototype)
		return;

	if (linkedTo) {	
		for (int i = 0; i < channels.size(); ++i)
			channels[i]->set3DAttributes(linkedTo->getPosition(), linkedTo->getVelocity());				
	}
	
	TGen::Engine::Sound::Source::update(dt);
}


void TGen::Engine::Sound::LocalSource::trigger(TGen::Engine::TriggerContext & context, TriggerMode mode) {
	TGen::Engine::Symbol methodSymbol = context.getFunctionSymbol();
	
	if (methodSymbol == symbolPlaySound) {
		uint32 soundId = *context.getParameter<uint32 *>(0);
		TGen::Engine::Sound::Sound * sound = reinterpret_cast<TGen::Engine::Sound::Sound *>(soundId);
		
		if (!sound)
			throw TGen::RuntimeException("Sound::Source::trigger", "NULL sound sent");
		
		TGen::Engine::Sound::Channel * newChannel = sound->spawnChannel(false);
		newChannel->set3D(true);		// det som är felet!		blir override i localsource... KOLLA SEN HUR CALL OCH MOVE kan förbättras så de bara använder ComponentLink!
		newChannel->set3DMinMaxDistance(minDistance, maxDistance);
		
		addChannel(newChannel);
	}
	else if (methodSymbol == symbolPlaySoundWithVolume) {
		uint32 soundId = *context.getParameter<uint32 *>(0);
		TGen::Engine::Sound::Sound * sound = reinterpret_cast<TGen::Engine::Sound::Sound *>(soundId);
		
		if (!sound)
			throw TGen::RuntimeException("Sound::Source::trigger", "NULL sound sent");
		
		TGen::Engine::Sound::Channel * newChannel = sound->spawnChannel(false);
		newChannel->set3D(true);		// det som är felet!		blir override i localsource... KOLLA SEN HUR CALL OCH MOVE kan förbättras så de bara använder ComponentLink!
		newChannel->set3DMinMaxDistance(minDistance, maxDistance);
		
		addChannel(newChannel);
	}
	else {
		TGen::Engine::Sound::Source::trigger(context, mode);
	}
	
}


TGen::Engine::Sound::Channel * TGen::Engine::Sound::LocalSource::spawnChannel(bool paused) {
	TGen::Engine::Sound::Channel * newChannel = TGen::Engine::Sound::Source::spawnChannel(paused);
	
	newChannel->set3D(true);
	newChannel->set3DMinMaxDistance(minDistance, maxDistance);
	
	return newChannel;
}

void TGen::Engine::Sound::LocalSource::setMinMaxDistance(scalar min, scalar max) {
	minDistance = min;
	maxDistance = max;
}

void TGen::Engine::Sound::LocalSource::setLink(TGen::Engine::WorldObject * link) {
	linkedTo = link;
}
