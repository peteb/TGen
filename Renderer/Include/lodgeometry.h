/*
 *  lodgeometry.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/14/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_LODGEOMETRY_H
#define _TGEN_RENDERER_LODGEOMETRY_H

#include "geometry.h"

namespace TGen {
	class LodGeometry : public TGen::Geometry {
	public:	
		LodGeometry(int steps);
		virtual ~LodGeometry();

		virtual void update(const TGen::Camera & camera, scalar distance, scalar time);
		virtual void lodUpdate(int level) abstract;
		
				
	private:
		int steps, lastLod;
	};
	
	
} // !TGen

#endif // !_TGEN_RENDERER_LODGEOMETRY_H
