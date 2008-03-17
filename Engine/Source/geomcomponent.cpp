/*
 *  geomcomponent.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/12/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "geomcomponent.h"
#include "bodycomponent.h"
#include "entity.h"
#include <tgen_core.h>

TGen::Engine::GeomComponent::GeomComponent(const std::string & name)
	: TGen::Engine::Component(name)
	, geomId(0)
{

}

TGen::Engine::GeomComponent::~GeomComponent() {
	
}

float TGen::Engine::GeomComponent::getFriction() const {
	return friction;
}

void TGen::Engine::GeomComponent::setFriction(float friction) {
	this->friction = friction;
}

void TGen::Engine::GeomComponent::setGeomId(dGeomID id) {
	if (geomId)
		throw TGen::RuntimeException("GeomComponent::setGeomId", "geomId already set!");
	
	geomId = id;
	dGeomSetData(geomId, static_cast<void *>(this));
}

void TGen::Engine::GeomComponent::linkLocally(TGen::Engine::Entity & entity) {
	try {
		TGen::Engine::BodyComponent * attachTo = dynamic_cast<TGen::Engine::BodyComponent *>(entity.getComponent("physBody"));
		dGeomSetBody(geomId, attachTo->getBodyId());
	}
	catch (...) {	// if there is no physBody we just don't attach to anything
		dGeomSetBody(geomId, 0);
		std::cout << "*** NO BODY *** " << std::endl;
	}
}

void TGen::Engine::GeomComponent::linkGlobally(TGen::Engine::EntityList & entities) {
	
}

