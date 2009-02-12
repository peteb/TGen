/*
 *  objectreference.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/25/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_UTILITIES_OBJECTREFERENCE_H
#define _TGEN_ENGINE_UTILITIES_OBJECTREFERENCE_H

#include "resourcecomponent.h"
#include "componentlink.h"
#include "componentrecipe.h"

namespace TGen {
	namespace Engine {
		namespace Utilities {
			class ObjectReference : public TGen::Engine::ResourceComponent, public TGen::Engine::ComponentRecipe {
			public:
				ObjectReference(const std::string & name);
				
				void * getData() const;
				void setObjectName(const std::string & name);

				void link(const TGen::Engine::ComponentLinker & linker);
				void prelink(const TGen::Engine::ComponentLinker & linker);
				
				TGen::Engine::Component * createComponent(const TGen::Engine::EntityRecipe & entity, TGen::Engine::Entity & constructing);
			};
		} // !Utilities
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_UTILITIES_OBJECTREFERENCE_H

