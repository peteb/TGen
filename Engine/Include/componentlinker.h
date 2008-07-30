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
			
			TGen::Engine::Component * getComponent(const std::string & name) const;
			TGen::Engine::Component * getComponent(int index) const;
			
			TGen::Engine::EntityList * getEntityList() const;
			TGen::Engine::Entity * getEntity() const;
			void setEntityList(TGen::Engine::EntityList * entityList);
			void setEntity(TGen::Engine::Entity * entity);
			
		private:
			TGen::Engine::EntityList * entityList;
			TGen::Engine::Entity * entity;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_COMPONENTLINKER_H

// TODO: name mangler för metoder, metodAndParam1:parameter2:parameter3:
//			name manglandet görs om till en symbol
//			börja med att parametrarna måste vara i rätt ordning

//			efter det så kan man fixa att ordningen av parametrarna inte spelar roll,
//			dock måste man mappa om till hur funktionen tar parametrarna då
//			fast man kanske måste ha symboler för varje parameter då, mappa parametersymbol till register...
