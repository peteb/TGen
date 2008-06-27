/*
 *  ccylindergeom.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/26/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "physics/ccylindergeom.h"

TGen::Engine::Physics::CCylinderGeom::CCylinderGeom(const std::string & name, scalar radius, scalar length, dSpaceID space) 
	: TGen::Engine::Physics::Geom(name)
{
	dGeomID newGeom = dCreateCCylinder(space, radius, length);
	setGeomId(newGeom);
}
