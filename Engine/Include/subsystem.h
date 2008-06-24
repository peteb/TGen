/*
 *  subsystem.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/5/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SUBSYSTEM_H
#define _TGEN_ENGINE_SUBSYSTEM_H

#include <tgen_core.h>

namespace TGen {
	namespace Engine {
		class Component;
		class ComponentRecipe;
		
		class Subsystem {
		public:
			virtual ~Subsystem() {}
			
			virtual TGen::Engine::Component * createComponent(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties) abstract;
			virtual TGen::Engine::ComponentRecipe * createComponentRecipe(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties) {throw TGen::RuntimeException("Subsystem::createComponentRecipe", "no implementation for " + name); }
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SUBSYSTEM_H
