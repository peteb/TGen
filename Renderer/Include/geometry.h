/*
 *  geometry.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/12/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_GEOMETRY_H
#define _TGEN_RENDERER_GEOMETRY_H

#include <tgen_graphics.h>

namespace TGen {
	class Geometry : public TGen::Renderable {
	public:	
		Geometry() {}
		virtual ~Geometry() {}
		
		virtual void preRenderer(TGen::Renderer & renderer) const abstract;		// set vb's, ib's
		virtual void render(TGen::Renderer & renderer) const abstract;
		virtual void update(/*const TGen::Camera & camera,*/ const TGen::Vector3 & position) {}	// LOD, billboards, etc
		
		virtual TGen::Vector3 getMax() const abstract;
		virtual TGen::Vector3 getMin() const abstract;
		
	};
	
	
} // !TGen

#endif // !_TGEN_RENDERER_GEOMETRY_H
