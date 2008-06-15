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
#include <map>

namespace TGen {
	namespace Engine {
		class ComponentRecipe;
		class EntityList;
		class Entity;
		
		class EntityRecipe {
		public:
			EntityRecipe(const std::string & name);
			virtual ~EntityRecipe(); 
			
			const std::string & getName() const;
			void addComponentRecipe(TGen::Engine::ComponentRecipe * recipe, const std::string & name);
			void linkGlobally(TGen::Engine::EntityList & entities);
			TGen::Engine::Entity * createEntity() const;
			TGen::Engine::ComponentRecipe * getComponentRecipe(const std::string & name);
			
		private:
			std::string name;
			
			typedef std::vector<TGen::Engine::ComponentRecipe *> RecipeList;
			typedef std::map<std::string, TGen::Engine::ComponentRecipe *> RecipeMap;
			
			RecipeList componentRecipes;
			RecipeMap recipeLookup;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_ENTITYRECIPE_H

