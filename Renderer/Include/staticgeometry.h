/*
 *  staticgeometry.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/14/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_STATICGEOMETRY_H
#define _TGEN_RENDERER_STATICGEOMETRY_H

#include "geometry.h"

namespace TGen {
	class Camera;
	
	class StaticGeometry : public TGen::Geometry {
	public:	
		StaticGeometry();
		virtual ~StaticGeometry();
		
		void update(const TGen::Camera & camera, scalar distance, scalar time);
	};
	
} // !TGen

#endif // !_TGEN_RENDERER_STATICGEOMETRY_H
