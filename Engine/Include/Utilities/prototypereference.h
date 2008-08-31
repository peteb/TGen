/*
 *  prototypereference.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/30/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_UTILITIES_PROTOTYPEREFERENCE_H
#define _TGEN_ENGINE_UTILITIES_PROTOTYPEREFERENCE_H

#include "resourcecomponent.h"
#include "componentrecipe.h"

namespace TGen {
	namespace Engine {
		class EntityRecipe;
		
		namespace Utilities {
			class PrototypeReference : public TGen::Engine::ResourceComponent, public TGen::Engine::ComponentRecipe {
			public:
				PrototypeReference(const std::string & name);

				void * getData() const;
				void setPrototypeName(const std::string & name);
				
				void link(const TGen::Engine::ComponentLinker & linker);
				void prelink(const TGen::Engine::ComponentLinker & linker);
				
				TGen::Engine::Component * createComponent(const TGen::Engine::EntityRecipe & entity, TGen::Engine::Entity & constructing);
				
			private:
				std::string prototypeName;
				TGen::Engine::EntityRecipe * prototype;
			};
			
		} // !Utilities
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_UTILITIES_PROTOTYPEREFERENCE_H

