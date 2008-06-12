/*
 *  entity.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/5/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "entity.h"
#include "component.h"
#include <tgen_core.h>

TGen::Engine::Entity::Entity(const std::string & name)
	: name(name)
{
	
}

TGen::Engine::Entity::~Entity() {
	
}

const std::string & TGen::Engine::Entity::getName() const {
	return name;
}

void TGen::Engine::Entity::addComponent(TGen::Engine::Component * component, const std::string & name) {
	std::cout << "ADDCOMP " << name << " to " << this->name << std::endl;
	components.insert(ComponentMap::value_type(name, component));
}
	
TGen::Engine::Component * TGen::Engine::Entity::getComponent(const std::string & name) {
	ComponentMap::iterator iter = components.find(name);
	
	if (iter == components.end())
		throw TGen::RuntimeException("Entity::getComponent", "no component called '" + name + "' loaded!");

	return iter->second;
}

void TGen::Engine::Entity::linkLocally() {
	// Links references to other components for this entity
	
	for (ComponentMap::iterator iter = components.begin(); iter != components.end(); ++iter)
		iter->second->linkLocally(*this);
}

void TGen::Engine::Entity::linkGlobally(TGen::Engine::EntityList & entities) {
	for (ComponentMap::iterator iter = components.begin(); iter != components.end(); ++iter)
		iter->second->linkGlobally(entities, *this);
}
