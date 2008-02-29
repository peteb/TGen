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
#include <map>

namespace TGen {
	namespace Engine {
		class Component;
		class EntityList;
		
		class Entity {
		public:
			Entity(const std::string & name);
			~Entity();
			
			const std::string & getName() const;
			void addComponent(TGen::Engine::Component * component, const std::string & name);
			TGen::Engine::Component * getComponent(const std::string & name);
			void linkLocally();
			void linkGlobally(TGen::Engine::EntityList & entities);
			
		private:
			typedef std::map<std::string, TGen::Engine::Component *> ComponentMap;
			
			//std::vector<TGen::Engine::Component *> components;
			ComponentMap components;
			
			std::string name;
		}; 
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_ENTITY_H
