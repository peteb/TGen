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
		class Subsystem;
		class StandardLogs;
		
		class EntityFactory {
		public:
			EntityFactory(TGen::Engine::StandardLogs & logs);
			~EntityFactory();
			
			TGen::Engine::Entity * createEntity(const TGen::PropertyTree & properties);
			void registerSubsystem(const std::string & componentName, TGen::Engine::Subsystem * subsystem);
			
		private:
			typedef std::map<std::string, TGen::Engine::Subsystem *> SubsystemMap;
			
			SubsystemMap subsystems;
			
			TGen::Engine::StandardLogs & logs;
		};
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_ENTITYFACTORY_H
