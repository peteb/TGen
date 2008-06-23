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
	componentLookup.insert(ComponentMap::value_type(name, component));
	components.push_back(component);
}
	
TGen::Engine::Component * TGen::Engine::Entity::getComponent(const std::string & name) {
	ComponentMap::iterator iter = componentLookup.find(name);
	
	if (iter == componentLookup.end())
		throw TGen::RuntimeException("Entity::getComponent", "no component called '" + name + "' loaded!");

	return iter->second;
}

TGen::Engine::Component * TGen::Engine::Entity::getComponent(int index) {
	return components.at(index);
}

void TGen::Engine::Entity::linkLocally() {
	// Links references to other components for this entity
	
	for (ComponentMap::iterator iter = componentLookup.begin(); iter != componentLookup.end(); ++iter)
		iter->second->linkLocally(*this);
}

void TGen::Engine::Entity::linkGlobally(TGen::Engine::EntityList & entities) {
	for (ComponentMap::iterator iter = componentLookup.begin(); iter != componentLookup.end(); ++iter)
		iter->second->linkGlobally(entities, *this);
}
