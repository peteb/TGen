/*
 *  spheregeomcomponent.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/1/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "spheregeomcomponent.h"

TGen::Engine::SphereGeomComponent::SphereGeomComponent(const std::string & name, float radius, dSpaceID space) 
	: TGen::Engine::GeomComponent(name)
{
	dGeomID newGeom = dCreateSphere(space, radius);
	setGeomId(newGeom);
}
