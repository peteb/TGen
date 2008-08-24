/*
 *  componentfactory.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/24/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_PHYSICS_COMPONENTFACTORY_H
#define _TGEN_ENGINE_PHYSICS_COMPONENTFACTORY_H

#include "ode/ode.h"
#include <string>
#include <tgen_core.h>

namespace TGen {
	class PropertyTree;
	
	namespace Engine {
		class Component;
		class ComponentRecipe;
		class Filesystem;
		
		namespace Physics {
			class Subsystem;
			class Body;
			class Geom;
			class Joint;
			
			class ComponentFactory {
			public:
				ComponentFactory(TGen::Engine::Physics::Subsystem & subsystem, TGen::Engine::Filesystem & filesystem);
				
				TGen::Engine::ComponentRecipe * createComponentRecipe(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties);

				TGen::Engine::Physics::Body * createBody(const std::string & name, const TGen::PropertyTree & properties, dWorldID worldId, dSpaceID spaceId);
				TGen::Engine::Physics::Geom * createGeom(const std::string & name, const TGen::PropertyTree & properties, dSpaceID mainSpace);
				TGen::Engine::Physics::Joint * createJoint(const std::string & name, const TGen::PropertyTree & properties, dWorldID worldId);

			private:
				static dMass getMass(const TGen::PropertyTree & properties);
				static uint getCategoryBits(const std::string & name);
	
				
				TGen::Engine::Physics::Subsystem & subsystem;
				TGen::Engine::Filesystem & filesystem;
			};
			
		} // !Physics
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_PHYSICS_COMPONENTFACTORY_H

