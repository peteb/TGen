/*
 *  raygeom.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/27/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "physics/geom.h"
#include "physics/raygeom.h"
#include <tgen_math.h>

TGen::Engine::Physics::RayGeom::RayGeom(const std::string & name, scalar length, dSpaceID space)
	: TGen::Engine::Physics::Geom(name)
{
	dGeomID newGeom = dCreateRay(space, length);
	
	setGeomId(newGeom);
}

void TGen::Engine::Physics::RayGeom::setOrientation(const TGen::Matrix3x3 & orientation) {
	TGen::Matrix3x3 fixedRotation = orientation;
	fixedRotation.setZ(-fixedRotation.getZ());
	
	TGen::Engine::Physics::Geom::setOrientation(fixedRotation);
}
