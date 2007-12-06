/*
 *  entity.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/5/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "entity.h"

TGen::Engine::Entity::Entity(const std::string & name)
	: name(name)
{
	
}

TGen::Engine::Entity::~Entity() {
	
}

const std::string & TGen::Engine::Entity::getName() const {
	return name;
}

void TGen::Engine::Entity::addComponent(TGen::Engine::Component * component) {
	components.push_back(component);
}
	
