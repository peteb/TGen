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
	class Camera;
	
	class Geometry : public TGen::Renderable {
	public:	
		Geometry() {}
		virtual ~Geometry() {}
		
		virtual void preRender(TGen::Renderer & renderer) const abstract;		// set vb's, ib's, etc.
		virtual void render(TGen::Renderer & renderer) const abstract;
		virtual void update(const TGen::Camera & camera, scalar distance, scalar time) {}	// LOD, billboards, etc. called on geoms in render list, animating meshes
		
		virtual TGen::Vector3 getMax() const abstract;
		virtual TGen::Vector3 getMin() const abstract;
		virtual TGen::Vector3 getOrigin() const abstract;
		
	};
	
	
} // !TGen

#endif // !_TGEN_RENDERER_GEOMETRY_H
