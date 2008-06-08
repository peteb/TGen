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

#include "entity.h"
#include "componentinterfaces.h"

TGen::Engine::Sound::LocalSource::LocalSource(const std::string & name, const std::string & filename, const std::string & linkWith)
	: TGen::Engine::Sound::Source(name, filename)
	, linkWith(linkWith)
	, follow(NULL)
	, minDistance(1.0)
	, maxDistance(10000.0)
{
	
}

void TGen::Engine::Sound::LocalSource::linkLocally(TGen::Engine::Entity & entity) {
	follow = dynamic_cast<TGen::Engine::ObjectInterface *>(entity.getComponent(linkWith));
}

void TGen::Engine::Sound::LocalSource::update(scalar dt) {
	TGen::Engine::Sound::Source::update(dt);
	
	if (follow) {
		for (int i = 0; i < channels.size(); ++i)
			channels[i]->set3DAttributes(follow->getPosition(), follow->getVelocity());				
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
