/*
 *  channel.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/2/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "sound/channel.h"
#include "fmod/fmod_errors.h"
#include <tgen_core.h>

TGen::Engine::Sound::Channel::Channel(FMOD::Channel * channel)
	: channel(channel)
{

}

TGen::Engine::Sound::Channel::~Channel() {
	if (!channel)
		return;
	
	FMOD_RESULT result = channel->stop();
	
	if (result != FMOD_OK)
		throw TGen::RuntimeException("Sound::~Channel", "failed to stop channel on dtor: ") << FMOD_ErrorString(result);
}

void TGen::Engine::Sound::Channel::setLoop(bool loop) {
	if (loop)
		channel->setMode(FMOD_LOOP_NORMAL);
	else
		channel->setMode(FMOD_LOOP_OFF);
}

void TGen::Engine::Sound::Channel::reset() {
	channel->setPosition(0, FMOD_TIMEUNIT_MS);
}
