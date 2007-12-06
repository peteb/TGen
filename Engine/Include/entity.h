/*
 *  entity.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/5/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_ENTITY_H
#define _TGEN_ENGINE_ENTITY_H

#include <string>
#include <vector>

namespace TGen {
	namespace Engine {
		class Component;
		
		class Entity {
		public:
			Entity(const std::string & name);
			~Entity();
			
			const std::string & getName() const;
			void addComponent(TGen::Engine::Component * component);
			
		private:
			std::vector<TGen::Engine::Component *> components;
			std::string name;
		}; 
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_ENTITY_H
