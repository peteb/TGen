/*
 *  entitylist.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 2/29/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "entitylist.h"
#include "entity.h"
#include <tgen_core.h>

TGen::Engine::EntityList::~EntityList() {
	for (EntityMap::iterator iter = entities.begin(); iter != entities.end(); ++iter)
		delete iter->second;
}


void TGen::Engine::EntityList::addEntity(TGen::Engine::Entity * entity) {
	entities.insert(EntityMap::value_type(entity->getName(), entity));
}

TGen::Engine::Entity * TGen::Engine::EntityList::getEntity(const std::string & name) {
	EntityMap::iterator iter = entities.find(name);
	if (iter == entities.end())
		throw TGen::RuntimeException("EntityList::getEntity", "entity '" + name + "' does not exist");
	
	return iter->second;
}

void TGen::Engine::EntityList::linkGlobally() {
	for (EntityMap::iterator iter = entities.begin(); iter != entities.end(); ++iter) {
		try {
			iter->second->linkGlobally(*this);
		} catch (const std::exception & e) {
			// TODO: output log, flytta ut renderer från game state!
		}
	}
}

/* RenderStateCats 
	1. shaders, z-states, blending, textures of different format & size ...
	2. constants, color, transforms
	3. textures of same size & format
	4. vbs of same format
*/
