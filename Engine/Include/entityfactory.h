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

#include <vector>

namespace TGen {
	class PropertyTree;
	
	namespace Engine {
		class Entity;
		class Subsystem;
		
		class EntityFactory {
		public:
			EntityFactory();
			~EntityFactory();
			
			TGen::Engine::Entity * createEntity(const TGen::PropertyTree & properties);
			void registerSubsystem(TGen::Engine::Subsystem * subsystem);
			
		private:
			std::vector<TGen::Engine::Subsystem *> subsystems;
		};
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_ENTITYFACTORY_H
