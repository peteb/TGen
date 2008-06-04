/*
 *  globalsource.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/30/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "sound/globalsource.h"

TGen::Engine::Sound::GlobalSource::GlobalSource(const std::string & name, const std::string & filename, bool stream)
	: TGen::Engine::Sound::Source(name, filename, stream)
{

}
