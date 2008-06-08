/*
 *  soundsource.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/26/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "sound/soundsource.h"
#include "sound/subsystem.h"
#include "sound/channel.h"
#include "sound/sound.h"

#include "fmod/fmod_errors.h"
#include "componentinterfaces.h"
#include "entity.h"

using TGen::Engine::Sound::Channel;

TGen::Engine::Sound::SoundSource::SoundSource(const std::string & name, FMOD::Sound * sound, TGen::Engine::Sound::Subsystem & subsystem, const std::string & linkWith) 
	: TGen::Engine::Component(name)
	, sound(sound)
	, subsystem(subsystem)
	, shouldPlay(false)
	, isPlaying(false)
	, singleChannel(false)
	, track(NULL)
	, linkWith(linkWith)
{
	
}

TGen::Engine::Sound::SoundSource::~SoundSource() {
	sound->release();
}

void TGen::Engine::Sound::SoundSource::linkLocally(TGen::Engine::Entity & entity) {
	track = dynamic_cast<TGen::Engine::ObjectInterface *>(entity.getComponent(linkWith));
}

void TGen::Engine::Sound::SoundSource::linkGlobally(TGen::Engine::EntityList & entities) {
	
}

void TGen::Engine::Sound::SoundSource::update(scalar dt) {
	if (shouldPlay && dt > 0.00001) {
		if (singleChannel && !isPlaying || !singleChannel) {
			spawnChannel();
		}
		
		shouldPlay = false;
	}
	
	if (isPlaying && track) {
		TGen::Vector3 p = track->getPosition();
		TGen::Vector3 v = track->getVelocity();
		
		FMOD_VECTOR pos, vel;
		
		pos.x = p.x;
		pos.y = p.y;
		pos.z = p.z;
		
		vel.x = v.x;
		vel.y = v.y;
		vel.z = v.z;
		
		for (int i = 0; i < channels.size(); ++i)
			channels[i]->set3DAttributes(&pos, &vel);
	}
}

void TGen::Engine::Sound::SoundSource::play() {
	shouldPlay = true;
}

void TGen::Engine::Sound::SoundSource::setSingleChannel(bool single) {
	singleChannel = single;
}

void TGen::Engine::Sound::SoundSource::spawnChannel() {
	FMOD::Channel * channel;
	FMOD_RESULT result;
	result = subsystem.getFmodSystem()->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
	
	channels.push_back(channel);
	
	if (result != FMOD_OK)
		throw TGen::RuntimeException("Sound::SoundSource::spawnChannel", "Failed to play sound: ") << FMOD_ErrorString(result);
	
	isPlaying = true;
}










TGen::Engine::Sound::Source::Source(const std::string & name, const std::string & filename) 
	: TGen::Engine::Component(name)
	, filename(filename)
	, autoplay(false)
{
}

TGen::Engine::Sound::Source::~Source() {
	for (int i = 0; i < channels.size(); ++i)
		delete channels[i];
}

void TGen::Engine::Sound::Source::link(TGen::Engine::Sound::Subsystem & linker) {
	linkedSound = linker.getSound(filename);
}

void TGen::Engine::Sound::Source::unlink() {
	linkedSound = NULL;
}

TGen::Engine::Sound::Channel * TGen::Engine::Sound::Source::spawnChannel(bool paused) {
	if (!linkedSound)
		throw TGen::RuntimeException("Sound::Source", "no sound linked for " + filename);
	
	TGen::Engine::Sound::Channel * ret = linkedSound->spawnChannel(paused);
	channels.push_back(ret);
	
	return ret;
}


void TGen::Engine::Sound::Source::update(scalar dt) {
	if (autoplay && dt >= 0.0001) {	// just to make sure that this isn't a pre-game update (where dts are <= 0.0)
		autoplay = false;		// only play once
		
		Channel * newChannel = spawnChannel(false);
		newChannel->setLoop(loop);		
	}
}

void TGen::Engine::Sound::Source::setAutoplay(bool autoplay) {
	this->autoplay = autoplay;
}

void TGen::Engine::Sound::Source::setLoop(bool loop) {
	this->loop = loop;
}





