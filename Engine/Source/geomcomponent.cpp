/*
 *  geomcomponent.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/12/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "physics/geom.h"
#include "physics/body.h"
#include "entity.h"
#include <tgen_core.h>

TGen::Engine::Physics::Geom::Geom(const std::string & name)
	: TGen::Engine::Component(name)
	, geomId(0)
{

}

TGen::Engine::Physics::Geom::~Geom() {
	
}

float TGen::Engine::Physics::Geom::getFriction() const {
	return friction;
}

void TGen::Engine::Physics::Geom::setFriction(float friction) {
	this->friction = friction;
}

void TGen::Engine::Physics::Geom::setGeomId(dGeomID id) {
	if (geomId)
		throw TGen::RuntimeException("Geom::setGeomId", "geomId already set!");
	
	geomId = id;
	dGeomSetData(geomId, static_cast<void *>(this));
}

void TGen::Engine::Physics::Geom::linkLocally(TGen::Engine::Entity & entity) {
	try {
		TGen::Engine::Physics::Body * attachTo = dynamic_cast<TGen::Engine::Physics::Body *>(entity.getComponent("physBody"));
		dGeomSetBody(geomId, attachTo->getBodyId());
	}
	catch (...) {	// if there is no physBody we just don't attach to anything
		dGeomSetBody(geomId, 0);
		std::cout << "*** NO BODY *** " << std::endl;
	}
}

void TGen::Engine::Physics::Geom::linkGlobally(TGen::Engine::EntityList & entities) {
	
}

