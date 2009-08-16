/*
 *  id3cmgeom.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/15/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include "physics/id3cmgeom.h"

TGen::Engine::Physics::Id3CMGeom::Id3CMGeom(const std::string & name) 
: TGen::Engine::Physics::Geom(name)
{
	
}

TGen::Engine::Physics::Id3CMGeom::~Id3CMGeom() {
	
}

void TGen::Engine::Physics::Id3CMGeom::setGeom(dGeomID id) {
	setGeomId(id);
}
