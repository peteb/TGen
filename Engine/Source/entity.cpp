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
#include "componentlinker.h"
#include <tgen_core.h>

TGen::Engine::Entity::Entity(const std::string & name)
	: name(name)
	, worldInterface(NULL)
	, scriptInterface(NULL)
{
}


TGen::Engine::Entity::~Entity() {
	for (int i = 0; i < components.size(); ++i) {
		if (components[i]->isStatic() == false)
			delete components[i];
	}
	
	delete scriptInterface;
}


void TGen::Engine::Entity::link(const TGen::Engine::ComponentLinker & linker) {
	TGen::Engine::ComponentLinker newLinker(linker);
	newLinker.setEntity(this);
	
	for (ComponentMap::iterator iter = componentLookup.begin(); iter != componentLookup.end(); ++iter) {
		try {
			iter->second->link(newLinker);
		} catch (const TGen::RuntimeException & err) {
			throw TGen::RuntimeException("Entity::link", "Failed to link component '" + getName() + ":" + iter->second->getName() + "':\n" + err.getWhere() + ": " + err.getDescription());
		}
	}
}

void TGen::Engine::Entity::onCreation() {
	std::cout << "Entity Initialize" << std::endl;
	
	if (scriptInterface)
		scriptInterface->onCreation();
}


void TGen::Engine::Entity::setWorldInterface(TGen::Engine::WorldObject * worldInterface) {
	this->worldInterface = worldInterface;
}

TGen::Engine::WorldObject * TGen::Engine::Entity::getWorldInterface() const {
	return worldInterface;
}

void TGen::Engine::Entity::setScriptInterface(TGen::Engine::Script::EntityScript * scriptInterface) {
	delete this->scriptInterface;
	this->scriptInterface = scriptInterface;
}

TGen::Engine::Script::EntityScript * TGen::Engine::Entity::getScriptInterface() const {
	return scriptInterface;
}

void TGen::Engine::Entity::addComponent(TGen::Engine::Component * component, const std::string & name) {
	std::cout << "ADDCOMP " << name << " to " << this->name << std::endl;
	componentLookup.insert(ComponentMap::value_type(name, component));
	components.push_back(component);
}
	

TGen::Engine::Component * TGen::Engine::Entity::getComponent(const std::string & name, std::nothrow_t noth) {
	ComponentMap::iterator iter = componentLookup.find(name);
	
	for (ComponentMap::iterator iter2 = componentLookup.begin(); iter2 != componentLookup.end(); ++iter2)
		std::cout << iter2->first << std::endl;
	
	if (iter == componentLookup.end())
		throw TGen::RuntimeException("Entity::getComponent", "no component called '" + name + "' loaded!");

	return iter->second;
}


TGen::Engine::Component & TGen::Engine::Entity::getComponent(const std::string & name) {
	ComponentMap::iterator iter = componentLookup.find(name);
	if (iter == componentLookup.end())
		throw TGen::RuntimeException("Entity::getComponent", "no component called '" + name + "' loaded!");
	
	assert(iter->second);
	
	return *iter->second;
}


TGen::Engine::Component * TGen::Engine::Entity::getComponent(int index, std::nothrow_t noth) {
	if (index < 0 || index >= components.size())
		return NULL;
	
	return components[index];
}


TGen::Engine::Component & TGen::Engine::Entity::getComponent(int index) {
	TGen::Engine::Component * ret = components.at(index);
	if (!ret)
		throw TGen::RuntimeException("Entity::getComponent(index)", "NULL component");
	
	return *ret;
}


const std::string & TGen::Engine::Entity::getName() const {
	return name;
}



