/*
 *  planegeomcomponent.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/1/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "planegeomcomponent.h"
#include <tgen_math.h>
#include "bodycomponent.h"
#include "entity.h"

TGen::Engine::PlaneGeomComponent::PlaneGeomComponent(const std::string & name, const TGen::Plane3 & plane, dSpaceID space) 
	: TGen::Engine::GeomComponent(name)
{
	geomId = dCreatePlane(space, plane.normal.x, plane.normal.y, plane.normal.z, plane.distance);
	dGeomSetData(geomId, static_cast<void *>(this));
}

TGen::Engine::PlaneGeomComponent::~PlaneGeomComponent() {
	dGeomDestroy(geomId);
}


void TGen::Engine::PlaneGeomComponent::linkLocally(TGen::Engine::Entity & entity) {
	try {
		TGen::Engine::BodyComponent * attachTo = dynamic_cast<TGen::Engine::BodyComponent *>(entity.getComponent("physBody"));
		dGeomSetBody(geomId, attachTo->getBodyId());
	}
	catch (...) {	// if there is no physBody we just don't attach to anything
		dGeomSetBody(geomId, 0);
		std::cout << "*** NO BODY *** " << std::endl;
	}
}

void TGen::Engine::PlaneGeomComponent::linkGlobally(TGen::Engine::EntityList & entities) {
	
}

