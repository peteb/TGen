/*
 *  localrecipe.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/23/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SOUND_LOCALRECIPE_H
#define _TGEN_ENGINE_SOUND_LOCALRECIPE_H

#include "componentrecipe.h"

namespace TGen {
	namespace Engine {
		namespace Sound {
			class Source;
			class Subsystem;
			
			class LocalRecipe : public TGen::Engine::ComponentRecipe {
			public:
				LocalRecipe(const std::string & name, TGen::Engine::Sound::Subsystem & soundSubsystem);
				
				TGen::Engine::Component * createComponent(const TGen::Engine::EntityRecipe & entity, TGen::Engine::Entity & constructing);
				void linkGlobally(TGen::Engine::EntityList & entities, TGen::Engine::EntityRecipe & entity);
				void fastLinkConstructed(TGen::Engine::Component & constructed, TGen::Engine::Entity & entity);
				
				void setLinkWith(const std::string & link);
				void setSoundPrototype(TGen::Engine::Sound::Source * source);
				
			private:
				std::string linkWithName;
				int componentLinkNum;
				
				TGen::Engine::Sound::Source * prototypeSource;
				TGen::Engine::Sound::Subsystem & soundSubsystem;
			};
			
		} // !Sound
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SOUND_LOCALRECIPE_H

