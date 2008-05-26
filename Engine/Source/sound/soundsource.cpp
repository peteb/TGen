/*
 *  soundsource.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/26/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "sound/soundsource.h"

TGen::Engine::Sound::SoundSource::SoundSource(const std::string & name, FMOD::Sound * sound) 
	: TGen::Engine::Component(name)
	, sound(sound)
{
	
}

TGen::Engine::Sound::SoundSource::~SoundSource() {
	
}

void TGen::Engine::Sound::SoundSource::linkLocally(TGen::Engine::Entity & entity) {
	
}

void TGen::Engine::Sound::SoundSource::linkGlobally(TGen::Engine::EntityList & entities) {
	
}

void TGen::Engine::Sound::SoundSource::update(scalar dt) {

}
