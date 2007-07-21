/*
 *  mesh.cpp
 *  The Quaker
 *
 *  Created by Peter Backman on 7/21/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "mesh.h"

void Mesh::PrepareRender(TGen::Renderer & renderer) const {
	
}

void Mesh::Render(TGen::Renderer & renderer) const {
	
}

TGen::Vector3 Mesh::getMax() const {
	return TGen::Vector3(1.0f, 1.0f, 1.0f);
}

TGen::Vector3 Mesh::getMin() const {
	return TGen::Vector3(-1.0f, -1.0f, -1.0f);	
}

