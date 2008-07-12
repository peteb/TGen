/*
 *  globalsource.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/30/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_GLOBALSOURCE_H
#define _TGEN_ENGINE_GLOBALSOURCE_H

#include "sound/soundsource.h"

namespace TGen {
	namespace Engine {
		namespace Sound {
			class Subsystem;
			
			class GlobalSource : public TGen::Engine::Sound::Source {
			public:
				GlobalSource(const std::string & name, const std::string & filename, TGen::Engine::Sound::Subsystem & creator);
				
			};
			
		} // !Sound
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_GLOBALSOURCE_H

