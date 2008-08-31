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

#include "triggerable.h"
#include "symbols.h"

#include <string>
#include <vector>
#include <map>

namespace TGen {
	namespace Engine {
		class ComponentRecipe;
		class EntityList;
		class Entity;
		class ComponentLinker;
		
		class EntityRecipe : public TGen::Engine::Triggerable {
		public:
			EntityRecipe(const std::string & name);
			virtual ~EntityRecipe(); 
			
			void addComponentRecipe(TGen::Engine::ComponentRecipe * recipe, const std::string & name);
			void prelink(const TGen::Engine::ComponentLinker & linker);
			void trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode);
			
			TGen::Engine::Entity *				createEntity() const;
			TGen::Engine::ComponentRecipe *	getComponentRecipe(const std::string & name, std::nothrow_t nth);
			TGen::Engine::ComponentRecipe &	getComponentRecipe(const std::string & name);
			int										getComponentIndex(const std::string & name) const;
			
			const std::string &	getName() const;
			void						setWorldInterface(const std::string & worldInterface);
			const std::string &	getWorldInterfaceName() const;
			int						getWorldInterfaceIndex() const;
			
		private:
			typedef std::vector<TGen::Engine::ComponentRecipe *> RecipeList;
			typedef std::map<std::string, TGen::Engine::ComponentRecipe *> RecipeMap;
			
			RecipeList componentRecipes;
			RecipeMap recipeLookup;

			std::string name, worldInterfaceName;
			int worldInterfaceIndex;

			static TGen::Engine::Symbol symbolCreateEntity;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_ENTITYRECIPE_H

