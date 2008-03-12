/*
 *  spheregeomcomponent.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/1/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "spheregeomcomponent.h"
#include "bodycomponent.h"
#include "entity.h"

TGen::Engine::SphereGeomComponent::SphereGeomComponent(const std::string & name, float radius, dSpaceID space) 
	: TGen::Engine::GeomComponent(name)
{
	geomId = dCreateSphere(space, radius);
	dGeomSetData(geomId, static_cast<void *>(this));
}

TGen::Engine::SphereGeomComponent::~SphereGeomComponent() {
	dGeomDestroy(geomId);
}

void TGen::Engine::SphereGeomComponent::linkLocally(TGen::Engine::Entity & entity) {
	try {
		TGen::Engine::BodyComponent * attachTo = dynamic_cast<TGen::Engine::BodyComponent *>(entity.getComponent("physBody"));
		dGeomSetBody(geomId, attachTo->getBodyId());
	}
	catch (...) {	// if there is no physBody we just don't attach to anything
		dGeomSetBody(geomId, 0);
		std::cout << "*** NO BODY *** " << std::endl;
	}	
}

void TGen::Engine::SphereGeomComponent::linkGlobally(TGen::Engine::EntityList & entities) {
	
}

