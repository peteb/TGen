/*
 *  boxgeomcomponent.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/2/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "boxgeomcomponent.h"
#include "bodycomponent.h"
#include "entity.h"
#include <tgen_math.h>

TGen::Engine::BoxGeomComponent::BoxGeomComponent(const std::string & name, const TGen::Vector3 & size, dSpaceID space)
	: TGen::Engine::GeomComponent(name)
{
	geomId = dCreateBox(space, size.x, size.y, size.z);
	dGeomSetData(geomId, static_cast<void *>(this));
}

TGen::Engine::BoxGeomComponent::~BoxGeomComponent() {
	dGeomDestroy(geomId);
}


void TGen::Engine::BoxGeomComponent::linkLocally(TGen::Engine::Entity & entity) {
	try {
		TGen::Engine::BodyComponent * attachTo = dynamic_cast<TGen::Engine::BodyComponent *>(entity.getComponent("physBody"));
		dGeomSetBody(geomId, attachTo->getBodyId());
	}
	catch (...) {	// if there is no physBody we just don't attach to anything
		dGeomSetBody(geomId, 0);
		std::cout << "*** NO BODY *** " << std::endl;
	}	
}

void TGen::Engine::BoxGeomComponent::linkGlobally(TGen::Engine::EntityList & entities) {
	
}

