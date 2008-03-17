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

TGen::Engine::PlaneGeomComponent::PlaneGeomComponent(const std::string & name, const TGen::Plane3 & plane, dSpaceID space) 
	: TGen::Engine::GeomComponent(name)
{	
	dGeomID newId = dCreatePlane(space, plane.normal.x, plane.normal.y, plane.normal.z, plane.distance);
	setGeomId(newId);
}

