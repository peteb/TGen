/*
 *  soundresource.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/20/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "sound/soundresource.h"
#include "sound/subsystem.h"

TGen::Engine::Sound::SoundResource::SoundResource(const std::string & name, TGen::Engine::Sound::Subsystem & subsystem)
	: TGen::Engine::ResourceComponent(name)
	, subsystem(subsystem)
	, linkedSound(NULL)
{
}

void * TGen::Engine::Sound::SoundResource::getData() const {
	std::cout << "SOUNDRES: GET DATA " << linkedSound << std::endl;
	return reinterpret_cast<void *>(linkedSound);
}

void TGen::Engine::Sound::SoundResource::setSoundName(const std::string & name) {
	resourceName = name;
}

void TGen::Engine::Sound::SoundResource::link(const TGen::Engine::ComponentLinker & linker) {
	std::cout << "SOUNDRES: get sound for " << resourceName << std::endl;
	linkedSound = subsystem.getSound(resourceName);
	std::cout << "IS " << linkedSound << std::endl;
}

