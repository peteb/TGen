/*
 *  entityfactory.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/5/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_ENTITYFACTORY_H
#define _TGEN_ENGINE_ENTITYFACTORY_H

#include <map>

namespace TGen {
	class PropertyTree;
	
	namespace Engine {
		class Entity;
		class EntityRecipe;
		class Subsystem;
		class StandardLogs;
		class Component;
		class ComponentRecipe;
		
		class EntityFactory {
		public:
			EntityFactory(TGen::Engine::StandardLogs & logs);
			~EntityFactory();
			
			TGen::Engine::Entity * createEntity(const TGen::PropertyTree & properties);
			void addClassEntity(const TGen::PropertyTree & properties);
			TGen::Engine::EntityRecipe * createPrototypeEntity(const TGen::PropertyTree & properties);
			
			void registerSubsystem(const std::string & componentName, TGen::Engine::Subsystem * subsystem);
			
		private:
			TGen::Engine::Component * createComponent(const std::string & entityName, const TGen::PropertyTree & properties) const;
			TGen::Engine::ComponentRecipe * createComponentRecipe(const std::string & entityName, const TGen::PropertyTree & properties) const;
			
			TGen::PropertyTree extendTree(const TGen::PropertyTree & base, const TGen::PropertyTree & entity) const;
			
			typedef std::map<std::string, TGen::Engine::Subsystem *> SubsystemMap;
			typedef std::map<std::string, TGen::PropertyTree> ClassMap;
			
			SubsystemMap subsystems;
			ClassMap classDefinitions;
			
			TGen::Engine::StandardLogs & logs;
		};
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_ENTITYFACTORY_H
