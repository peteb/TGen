/*
 *  entityrecipe.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/12/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_ENTITYRECIPE_H
#define _TGEN_ENGINE_ENTITYRECIPE_H

#include <string>
#include <vector>

namespace TGen {
	namespace Engine {
		class ComponentRecipe;
		class EntityList;
		
		class EntityRecipe {
		public:
			EntityRecipe(const std::string & name);
			virtual ~EntityRecipe(); 
			
			const std::string & getName() const;
			void addComponentRecipe(TGen::Engine::ComponentRecipe * recipe, const std::string & name);
			void linkGlobally(TGen::Engine::EntityList & entities);
			
		private:
			std::string name;
			
			typedef std::vector<TGen::Engine::ComponentRecipe *> RecipeList;
			RecipeList componentRecipes;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_ENTITYRECIPE_H

