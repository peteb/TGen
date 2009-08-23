/*
 *  component.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/18/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "component.h"
#include "entity.h"
#include "componentlinker.h"
#include <tgen_core.h>

TGen::Engine::Component::Component(const std::string & name, bool staticComponent) 
	: name(name)
	, owner(NULL)
	, parent(NULL)
	, staticComponent(staticComponent)
	, scriptInterface(NULL)
{
	
}

TGen::Engine::Component::~Component() {
	// do NOT remove script interface here; it will result in multiple deletes on same data
}

void TGen::Engine::Component::link(const TGen::Engine::ComponentLinker & linker) {
	if (linker.getEntity()) {
		owner = linker.getEntity();
	}
}

const std::string & TGen::Engine::Component::getName() const {
	return name;
}

void TGen::Engine::Component::setOwner(TGen::Engine::Entity * owner) {
	this->owner = owner;
}

/*TGen::Engine::Entity * TGen::Engine::Component::getOwner() const {
	return owner;
}*/

bool TGen::Engine::Component::isStatic() const {
	return staticComponent;
}

void TGen::Engine::Component::addSubcomponent(TGen::Engine::Component * component) {
	if (component->parent)
		throw TGen::RuntimeException("Component::addSubcomponent", "component already has a parent");
	
	children.push_back(component);
	component->parent = this;
}

TGen::Engine::Component * TGen::Engine::Component::getParent() const {
	return parent;
}

void TGen::Engine::Component::setScriptInterface(TGen::Engine::Script::ComponentScript * scriptInterface) {
	if (this->scriptInterface)
		throw TGen::RuntimeException("Component::setScriptInterface", "interface already set");
	
	this->scriptInterface = scriptInterface;
}

TGen::Engine::Script::ComponentScript * TGen::Engine::Component::getScriptInterface() {
	return scriptInterface;
}

int TGen::Engine::Component::getNumChildren() const {
	return children.size();
}

TGen::Engine::Component * TGen::Engine::Component::getChild(int id) {
	return children.at(id);
}

