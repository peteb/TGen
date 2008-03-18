/*
 *  spheregeomcomponent.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/1/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "physics/spheregeom.h"

TGen::Engine::Physics::SphereGeom::SphereGeom(const std::string & name, float radius, dSpaceID space) 
	: TGen::Engine::Physics::Geom(name)
{
	dGeomID newGeom = dCreateSphere(space, radius);
	setGeomId(newGeom);
}
