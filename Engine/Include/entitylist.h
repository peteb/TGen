/*
 *  entitylist.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 2/29/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_ENTITYLIST_H
#define _TGEN_ENGINE_ENTITYLIST_H

#include <string>
#include <map>

namespace TGen {
	namespace Engine {
		class Entity;
		class Component;
		class EntityRecipe;
		class ComponentLinker;
		
		class EntityList {
		public:
			~EntityList();
			
			void link();
			void addEntity(TGen::Engine::Entity * entity);
			void addPrototype(TGen::Engine::EntityRecipe * recipe);
			
			TGen::Engine::Entity * getEntity(const std::string & name, std::nothrow_t noth);
			TGen::Engine::Entity & getEntity(const std::string & name);

			TGen::Engine::EntityRecipe * getPrototype(const std::string & name, std::nothrow_t nth);
			TGen::Engine::EntityRecipe & getPrototype(const std::string & name);

			TGen::Engine::Component * getComponent(const std::string & name, TGen::Engine::Entity & from, std::nothrow_t noth);
			TGen::Engine::Component & getComponent(const std::string & name, TGen::Engine::Entity & from);
			
			TGen::Engine::Component * getComponent(const std::string & name, std::nothrow_t noth);
			TGen::Engine::Component & getComponent(const std::string & name);
						
		private:
			typedef std::map<std::string, TGen::Engine::Entity *> EntityMap;
			typedef std::map<std::string, TGen::Engine::EntityRecipe *> PrototypeMap;

			EntityMap entities;
			PrototypeMap prototypes;
		};
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_ENTITY_LIST_H
