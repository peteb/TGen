/*
 *  bodyrecipe.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/24/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_BODYRECIPE_H
#define _TGEN_ENGINE_BODYRECIPE_H

#include "componentrecipe.h"
#include <ODE/ODE.h>

namespace TGen {
	namespace Engine {
		namespace Physics {
			class Subsystem;
			
			class BodyRecipe : public TGen::Engine::ComponentRecipe {
			public:
				BodyRecipe(const std::string & name, dSpaceID space, dMass mass, TGen::Engine::Physics::Subsystem & subsystem);

				void link(const TGen::Engine::ComponentLinker & linker, TGen::Engine::EntityRecipe & entity);
				void fastLinkConstructed(TGen::Engine::Component & constructed, TGen::Engine::Entity & entity);	
				
				TGen::Engine::Component * createComponent(const TGen::Engine::EntityRecipe & entity, TGen::Engine::Entity & constructing);
				void setLink(const std::string & name);
				
			private:
				TGen::Engine::Physics::Subsystem & subsystem;
				dSpaceID space;
				
				std::string linkName;
				int linkComponentNum;
				dMass mass;
			};
			
		} // !Physics
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_BODYRECIPE_H

