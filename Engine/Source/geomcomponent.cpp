/*
 *  geomcomponent.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/12/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "geomcomponent.h"

TGen::Engine::GeomComponent::GeomComponent(const std::string & name)
	: TGen::Engine::Component(name) 
{

}

float TGen::Engine::GeomComponent::getFriction() const {
	return friction;
}

void TGen::Engine::GeomComponent::setFriction(float friction) {
	this->friction = friction;
}

