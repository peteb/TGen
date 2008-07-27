/*
 *  componentlinker.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/27/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_COMPONENTLINKER_H
#define _TGEN_ENGINE_COMPONENTLINKER_H

#include <string>

namespace TGen {
	namespace Engine {
		class EntityList;
		class Entity;
		class Component;
		
		class ComponentLinker {
		public:			
			ComponentLinker(TGen::Engine::EntityList * entityList, TGen::Engine::Entity * entity);
			
			TGen::Engine::Component * getGlobalComponent(const std::string & name) const;
			TGen::Engine::Component * getEntityComponent(int index) const;
			
			TGen::Engine::EntityList * getEntityList() const;
			TGen::Engine::Entity * getEntity() const;
			
		private:
			TGen::Engine::EntityList * entityList;
			TGen::Engine::Entity * entity;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_COMPONENTLINKER_H

