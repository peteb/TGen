/*
 *  channel.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/2/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include <tgen_math.h>

#include "sound/channel.h"
#include "sound/convert.h"

#include "fmod/fmod_errors.h"

TGen::Engine::Symbol TGen::Engine::Sound::Channel::symbolsSetUpdateVelocity = TGen::Engine::getUniqueSymbol("setUpdateVelocity:");

TGen::Engine::Sound::Channel::Channel(FMOD::Channel * channel)
	: channel(channel)
	, updateVelocity(true)
{

}

TGen::Engine::Sound::Channel::~Channel() {
	if (!channel)
		return;
	
	FMOD_RESULT result = channel->stop();
	
	//if (result != FMOD_OK)	// commented because you shouldn't throw in a dtor.
	//	throw TGen::RuntimeException("Sound::~Channel", "failed to stop channel on dtor: ") << FMOD_ErrorString(result);
}

void TGen::Engine::Sound::Channel::setLoop(bool loop) {
	if (loop)
		channel->setMode(FMOD_LOOP_NORMAL);
	else
		channel->setMode(FMOD_LOOP_OFF);
	
}

void TGen::Engine::Sound::Channel::set3D(bool enable) {
	if (enable)
		channel->setMode(FMOD_3D);
	else
		channel->setMode(FMOD_2D);
}

void TGen::Engine::Sound::Channel::setPaused(bool paused) {
	channel->setPaused(paused);
}

void TGen::Engine::Sound::Channel::reset() {
	channel->setPosition(0, FMOD_TIMEUNIT_MS);
}

void TGen::Engine::Sound::Channel::set3DAttributes(const TGen::Vector3 & position, const TGen::Vector3 & velocity) {
	FMOD_VECTOR pos, vel;
	
	TGen::Engine::Sound::TVecToFVec(position, pos);
	
	if (updateVelocity) {
		TGen::Engine::Sound::TVecToFVec(velocity, vel);
	}
	else {
		vel.x = vel.y = vel.z = 0.0f;
	}
	
	channel->set3DAttributes(&pos, &vel);
}

void TGen::Engine::Sound::Channel::set3DMinMaxDistance(scalar minDistance, scalar maxDistance) {
	channel->set3DMinMaxDistance(minDistance, maxDistance);
}

bool TGen::Engine::Sound::Channel::isPlaying() {
	bool ret = false;
	
	FMOD_RESULT result = channel->isPlaying(&ret);
	if (result != FMOD_OK)
		ret = false;
	
	return ret;
}

bool TGen::Engine::Sound::Channel::isPaused() {
	bool ret = false;
	
	channel->getPaused(&ret);
	
	return ret;
}

void TGen::Engine::Sound::Channel::setUpdateVelocity(bool updateVelocity) {
	this->updateVelocity = updateVelocity;
}

void TGen::Engine::Sound::Channel::trigger(TGen::Engine::TriggerContext & context, TriggerMode mode) {
	TGen::Engine::Symbol function = context.getFunctionSymbol();
	
	if (function == symbolsSetUpdateVelocity) {
		bool updateVelocity = *context.getParameter<bool *>(0);
		
		setUpdateVelocity(updateVelocity);
	}
}

