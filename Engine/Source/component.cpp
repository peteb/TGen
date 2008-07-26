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

TGen::Engine::Component::Component(const std::string & name, bool staticComponent) 
	: name(name)
	, owner(NULL)
	, staticComponent(staticComponent)
{
	
}

TGen::Engine::Component::~Component() {
	
}

void TGen::Engine::Component::linkLocally(TGen::Engine::Entity & entity) {
	context = &entity.getContext();
}

void TGen::Engine::Component::linkGlobally(TGen::Engine::EntityList & entities, TGen::Engine::Entity & entity) {
	
}

const std::string & TGen::Engine::Component::getName() const {
	return name;
}

void TGen::Engine::Component::trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode) {
	*context.getRegister<int *>(0) = -1;	// r0 is to be marked -1 if the method couldn't be called
}

void TGen::Engine::Component::setOwner(TGen::Engine::Entity * owner) {
	this->owner = owner;
}

TGen::Engine::Entity * TGen::Engine::Component::getOwner() const {
	return owner;
}

bool TGen::Engine::Component::isStatic() const {
	return staticComponent;
}

