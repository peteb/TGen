/*
 *  localsource.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/30/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_LOCALSOURCE_H
#define _TGEN_ENGINE_LOCALSOURCE_H

#include "../component.h"
#include "sound/soundsource.h"

namespace TGen {
	namespace Engine {
		namespace Sound {
			class LocalSource : public TGen::Engine::Sound::Source {
			public:
				LocalSource(const std::string & name, const std::string & filename);
				
			};
			
		} // !Sound
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_LOCALSOURCE_H

