/*
 *  subsystem.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/26/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "sound/subsystem.h"

TGen::Engine::Sound::Subsystem::Subsystem() {
	
}

TGen::Engine::Sound::Subsystem::~Subsystem() {
	
}


TGen::Engine::Component * TGen::Engine::Sound::Subsystem::createComponent(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties) {
	throw TGen::RuntimeException("blah", "blah");
	
	return NULL;
}

