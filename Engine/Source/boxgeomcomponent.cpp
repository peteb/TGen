/*
 *  boxgeomcomponent.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/2/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "physics/boxgeom.h"
#include <tgen_math.h>

TGen::Engine::Physics::BoxGeom::BoxGeom(const std::string & name, const TGen::Vector3 & size, dSpaceID space)
	: TGen::Engine::Physics::Geom(name)
{
	dGeomID newGeom = dCreateBox(space, size.x, size.y, size.z);
	setGeomId(newGeom);
}

