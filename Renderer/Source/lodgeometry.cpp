/*
 *  lodgeometry.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/14/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "lodgeometry.h"
#include "camera.h"

TGen::LodGeometry::LodGeometry(int steps)
	: steps(steps)
	, lastLod(-1)
{
}

TGen::LodGeometry::~LodGeometry() {
}

void TGen::LodGeometry::update(const TGen::Camera & camera, scalar distance, scalar time) {
	int lod = steps - int(((distance - camera.getLod().lodNear) / camera.getLod().lodFar) * steps);
	TGen::Clamp(lod, 0, steps - 1);
	
	if (lod != lastLod) {
		lodUpdate(lod);
		lastLod = lod;
	}
}

