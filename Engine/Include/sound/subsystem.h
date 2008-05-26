/*
 *  subsystem.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/26/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SOUND_SUBSYSTEM_H
#define _TGEN_ENGINE_SOUND_SUBSYSTEM_H

#include "../subsystem.h"

namespace TGen {
	namespace Engine {
		class StandardLogs;
		
		namespace Sound {
			class Subsystem : public TGen::Engine::Subsystem {
			public:	
				Subsystem(TGen::Engine::StandardLogs & logs);
				~Subsystem();
				
				TGen::Engine::Component * createComponent(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties);
				
			private:
				TGen::Engine::StandardLogs & logs;
			};
			
		} // !Sound
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SUBSYSTEM_H

