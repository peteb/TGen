/*
 *  eventrecipe.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/25/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SCRIPT_EVENTRECIPE_H
#define _TGEN_ENGINE_SCRIPT_EVENTRECIPE_H

#include "componentrecipe.h"

namespace TGen {
	namespace Engine {
		namespace Script {
			class Event;
			
			class EventRecipe : public TGen::Engine::ComponentRecipe {
			public:
				EventRecipe(const std::string & name, TGen::Engine::Script::Event * event);
				~EventRecipe();
				
				void prelink(const TGen::Engine::ComponentLinker & linker);
				void link(const TGen::Engine::ComponentLinker & linker);
				
				TGen::Engine::Component * createComponent(const TGen::Engine::EntityRecipe & entity, TGen::Engine::Entity & constructing);
				
			private:
				TGen::Engine::Script::Event * event;				
			};
			
		} // !Script
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCRIPT_EVENTRECIPE_H

