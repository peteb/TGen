/*
 *  globalsource.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/30/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "sound/globalsource.h"
#include "sound/sound.h"

TGen::Engine::Sound::GlobalSource::GlobalSource(const std::string & name, const std::string & filename, TGen::Engine::Sound::Subsystem & creator)
	: TGen::Engine::Sound::Source(name, filename, creator)
{

}

void TGen::Engine::Sound::GlobalSource::playSound(TGen::Engine::Sound::Sound * sound) {
	TGen::Engine::Sound::Channel * newChannel = sound->spawnChannel(false);
	
	newChannel->set3D(false);		// det som är felet!		blir override i localsource... KOLLA SEN HUR CALL OCH MOVE kan förbättras så de bara använder ComponentLink!
	//newChannel->set3DMinMaxDistance(minDistance, maxDistance);
	newChannel->setVolume(volume);
	
	addChannel(newChannel);
}