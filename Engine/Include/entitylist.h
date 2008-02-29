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
		
		class EntityList {
		public:
			~EntityList();
			
			void addEntity(TGen::Engine::Entity * entity);
			TGen::Engine::Entity * getEntity(const std::string & name);
			void linkGlobally();
			
		private:
			typedef std::map<std::string, TGen::Engine::Entity *> EntityMap;
			
			EntityMap entities;
		};
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_ENTITY_LIST_H
