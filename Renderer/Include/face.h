/*
 *  face.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_FACE_H
#define _TGEN_RENDERER_FACE_H

#include <tgen_math.h>

namespace TGen {
	class Face {
	public:
		
		TGen::Vector3 getMin() const;
		TGen::Vector3 getMax() const;
	}; 
	
	
} // !TGen

#endif // !_TGEN_RENDERER_FACE_H
