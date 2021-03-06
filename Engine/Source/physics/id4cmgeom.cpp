/*
 *  id4cmgeom.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/17/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "physics/id4cmgeom.h"

TGen::Engine::Physics::Id4CMGeom::Id4CMGeom(const std::string & name) 
	: TGen::Engine::Physics::Geom(name)
{
	
}

TGen::Engine::Physics::Id4CMGeom::~Id4CMGeom() {
	
}

void TGen::Engine::Physics::Id4CMGeom::setGeom(dGeomID id) {
	setGeomId(id);
}
