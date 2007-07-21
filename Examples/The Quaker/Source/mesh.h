/*
 *  mesh.h
 *  The Quaker
 *
 *  Created by Peter Backman on 7/21/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _THEQUAKER_MESH_H
#define _THEQUAKER_MESH_H

#include "geometry.h"

// TODO: this is not in use yet

class Mesh : public Geometry {
public:
	
	void PrepareRender(TGen::Renderer & renderer) const;
	void Render(TGen::Renderer & renderer) const;
	TGen::Vector3 getMax() const;
	TGen::Vector3 getMin() const;
	
};

#endif // !_THEQUAKER_MESH_H
