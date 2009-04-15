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
#include <vector>
#include "script/entityscript.h"

namespace TGen {
	namespace Engine {
		class Component;
		class EntityList;
		class ComponentLinker;
		class WorldObject;
		
		class Entity {
		public:
			Entity(const std::string & name);
			~Entity();
			
			void link(const TGen::Engine::ComponentLinker & linker);
			void addComponent(TGen::Engine::Component * component, const std::string & name);
			void onCreation();
			void setWorldInterface(TGen::Engine::WorldObject * worldInterface);
			void setScriptInterface(TGen::Engine::Script::EntityScript * scriptInterface);
			
			TGen::Engine::Script::EntityScript * getScriptInterface() const;
			TGen::Engine::WorldObject * getWorldInterface() const;
			
			TGen::Engine::Component * getComponent(const std::string & name, std::nothrow_t noth);
			TGen::Engine::Component & getComponent(const std::string & name);
			
			TGen::Engine::Component * getComponent(int index, std::nothrow_t noth);
			TGen::Engine::Component & getComponent(int index);

			const std::string & getName() const;
			
		private:
			typedef std::map<std::string, TGen::Engine::Component *> ComponentMap;

			std::vector<TGen::Engine::Component *> components;
			ComponentMap componentLookup;			

			std::string name;
			TGen::Engine::WorldObject * worldInterface;
			TGen::Engine::Script::EntityScript * scriptInterface;
		}; 
		
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_ENTITY_H
