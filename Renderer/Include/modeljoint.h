/*
 *  modeljoint.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/3/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MODELJOINT_H
#define _TGEN_RENDERER_MODELJOINT_H

#include <tgen_math.h>

namespace TGen {
	struct ModelJoint {
		TGen::Vector3 origin;
		TGen::Rotation orientation;		
	};	
	
} // !TGen

#endif // !_TGEN_RENDERER_MODELJOINT_H
