/*
 *  planegeomcomponent.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/1/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "physics/planegeom.h"
#include <tgen_math.h>

TGen::Engine::Physics::PlaneGeom::PlaneGeom(const std::string & name, const TGen::Plane3 & plane, dSpaceID space) 
	: TGen::Engine::Physics::Geom(name)
{	
	dGeomID newId = dCreatePlane(space, plane.normal.x, plane.normal.y, plane.normal.z, plane.distance);
	setGeomId(newId);
}

