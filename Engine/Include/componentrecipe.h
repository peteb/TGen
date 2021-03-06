/*
 *  componentrecipe.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/12/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_COMPONENTRECIPE_H
#define _TGEN_ENGINE_COMPONENTRECIPE_H

#include <tgen_core.h>

namespace TGen {
	namespace Engine {
		class Component;
		class EntityList;
		class EntityRecipe;
		class Entity;
		class ComponentLinker;
		
		class ComponentRecipe {
		public:
			ComponentRecipe(const std::string & name)
				: name(name)
			{
			}
			
			virtual ~ComponentRecipe() {}
			
			virtual void prelink(const TGen::Engine::ComponentLinker & linker) {}
			virtual void link(const TGen::Engine::ComponentLinker & linker) {}

			virtual TGen::Engine::Component * createComponent(const TGen::Engine::EntityRecipe & entity, TGen::Engine::Entity & constructing) abstract;
			
			const std::string & getName() const {return name; }
			
		protected:
			const std::string name;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_COMPONENTRECIPE_H

