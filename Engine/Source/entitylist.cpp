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
#include "entityrecipe.h"
#include <tgen_core.h>

TGen::Engine::EntityList::~EntityList() {
	for (EntityMap::iterator iter = entities.begin(); iter != entities.end(); ++iter)
		delete iter->second;
	
	for (PrototypeMap::iterator iter = prototypes.begin(); iter != prototypes.end(); ++iter)
		delete iter->second;
}


void TGen::Engine::EntityList::addEntity(TGen::Engine::Entity * entity) {
	entities.insert(std::make_pair(entity->getName(), entity));
}

void TGen::Engine::EntityList::addPrototype(TGen::Engine::EntityRecipe * recipe) {
	prototypes.insert(std::make_pair(recipe->getName(), recipe));
}

TGen::Engine::Entity * TGen::Engine::EntityList::getEntity(const std::string & name) {
	EntityMap::iterator iter = entities.find(name);
	if (iter == entities.end())
		throw TGen::RuntimeException("EntityList::getEntity", "entity '" + name + "' does not exist");
	
	return iter->second;
}

TGen::Engine::EntityRecipe * TGen::Engine::EntityList::getPrototype(const std::string & name) {
	PrototypeMap::iterator iter = prototypes.find(name);
	if (iter == prototypes.end())
		throw TGen::RuntimeException("EntityList::getPrototype", "prototype '" + name + "' does not exist");
	
	return iter->second;
}

void TGen::Engine::EntityList::linkGlobally() {
	for (EntityMap::iterator iter = entities.begin(); iter != entities.end(); ++iter) {
		//try {
			iter->second->linkGlobally(*this);
		//} catch (const std::exception & e) {
			
			// TODO: output log, flytta ut renderer från game state! Huh?
		//}
	}
	
	for (PrototypeMap::iterator iter = prototypes.begin(); iter != prototypes.end(); ++iter)
		iter->second->linkGlobally(*this);
}

TGen::Engine::Component * TGen::Engine::EntityList::getComponent(const std::string & name, TGen::Engine::Entity & from) {
	std::string entityName;
	std::string componentName;
	
	int colonPos = name.find(":");
	
	if (colonPos != std::string::npos) {
		entityName = name.substr(0, colonPos);
		componentName = name.substr(colonPos + 1);
	}
	else {
		componentName = name;
	}

	if (entityName.empty()) {
		from.getComponent(componentName);
		return from.getComponent(componentName);
	}
	else {
		return getEntity(entityName)->getComponent(componentName);
	}

	return NULL;
}

TGen::Engine::Component * TGen::Engine::EntityList::getComponent(const std::string & name) {
	std::string entityName;
	std::string componentName;
	
	int colonPos = name.find(":");
	
	if (colonPos != std::string::npos) {
		entityName = name.substr(0, colonPos);
		componentName = name.substr(colonPos + 1);
	}
	else {
		return NULL;
	}
	
	return getEntity(entityName)->getComponent(componentName);
}

/* RenderStateCats 
	1. shaders, z-states, blending, textures of different format & size ...
	2. constants, color, transforms
	3. textures of same size & format
	4. vbs of same format
*/
