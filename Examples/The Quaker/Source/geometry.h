/*
 *  geometry.h
 *  The Quaker
 *
 *  Created by Peter Backman on 7/18/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _THEQUAKER_GEOMETRY_H
#define _THEQUAKER_GEOMETRY_H

#include <tgen_graphics.h>

class Geometry : public TGen::Renderable {
public:	
	Geometry() {}
	virtual ~Geometry() {}
	
	virtual void PrepareRender(TGen::Renderer & renderer) const abstract; // once per geometry
	virtual void Render(TGen::Renderer & renderer) const abstract; // once per pass
	virtual TGen::Vector3 getMax() const abstract;
	virtual TGen::Vector3 getMin() const abstract;
};



#endif // !_THEQUAKER_GEOMETRY_H
