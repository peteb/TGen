/*
 *  geometry.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/23/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "geometry.h"

const TGen::Geometry::SubfaceList * TGen::Geometry::getLeaves() const {
	if (!leaves.empty())
		return &leaves;
	
	return NULL;
}

void TGen::Geometry::addLeaf(TGen::Subface * leaf) {
	leaves.push_back(leaf);
}
