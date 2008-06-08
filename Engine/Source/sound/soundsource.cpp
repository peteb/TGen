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
	
	for (ChannelList::iterator iter = channels.begin(); iter != channels.end();) {
		if (!(*iter)->isPlaying())
			iter = channels.erase(iter);
		else
			++iter;
	}
}

void TGen::Engine::Sound::Source::setAutoplay(bool autoplay) {
	this->autoplay = autoplay;
}

void TGen::Engine::Sound::Source::setLoop(bool loop) {
	this->loop = loop;
}



