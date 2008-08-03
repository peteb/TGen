/*
 *  staticcomponentrecipe.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/25/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_STATICCOMPONENTRECIPE_H
#define _TGEN_ENGINE_STATICCOMPONENTRECIPE_H

#include "componentrecipe.h"

namespace TGen {
	namespace Engine {
		class Component;
		class ComponentLinker;
		
		// TODO: is anything using this????? yep!
		
		class StaticComponentRecipe : public TGen::Engine::ComponentRecipe {
		public:
			StaticComponentRecipe(const std::string & name, TGen::Engine::Component * component)
				: TGen::Engine::ComponentRecipe(name)
				, component(component)
			{
			}
			
			~StaticComponentRecipe() {
				delete component;
			}
			
			void fastLinkConstructed(const TGen::Engine::ComponentLinker & linker, TGen::Engine::Component & constructed) {
				constructed.setOwner(reinterpret_cast<TGen::Engine::Entity *>(0xBEEF));	// TODO: might not be a good idea to use pointers
			}
			
			void link(const TGen::Engine::ComponentLinker & linker, TGen::Engine::EntityRecipe & prototype) {
				if (component)
					component->link(linker);
			}
			
			TGen::Engine::Component * createComponent(const TGen::Engine::EntityRecipe & entity, TGen::Engine::Entity & constructing) {
				return component;
			}
			
		private:
			TGen::Engine::Component * component;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_STATICCOMPONENTRECIPE_H

