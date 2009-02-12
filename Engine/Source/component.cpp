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

TGen::Engine::Component::Component(const std::string & name, bool staticComponent) 
	: name(name)
	, owner(NULL)
	, staticComponent(staticComponent)
{
	
}

TGen::Engine::Component::~Component() {
	
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

