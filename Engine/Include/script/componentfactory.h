/*
 *  componentfactory.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/18/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SCRIPT_COMPONENTFACTORY_H
#define _TGEN_ENGINE_SCRIPT_COMPONENTFACTORY_H

#include <string>

namespace TGen {
	class PropertyTree;

	namespace Engine {
		class ComponentRecipe;
		class Component;
		class Entity;
		
		namespace Script {
			class Subsystem;
			class Component;
			
			class ComponentFactory {
			public:
				ComponentFactory(TGen::Engine::Script::Subsystem & subsystem);
				
				TGen::Engine::Component * createComponent(const std::string & name, TGen::Engine::Entity & entity, const TGen::PropertyTree & properties);
				TGen::Engine::ComponentRecipe * createComponentRecipe(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties);
				
			private:
				TGen::Engine::Script::Subsystem & subsystem;
			};
			
		} // !Script
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCRIPT_COMPONENTFACTORY_H

