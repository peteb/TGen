/*
 *  AABB.h
 *  TGen Math
 *
 *  Created by Peter Backman on 7/18/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_AABB_H
#define _TGEN_AABB_H

#include "vector3.h"

namespace TGen {
	class AABB {
	public:	
		AABB();
		AABB(const TGen::Vector3 & center, scalar width, scalar height, scalar depth);
		AABB(const TGen::Vector3 & min, const TGen::Vector3 & max);
		
		void Calculate(const TGen::Vector3 & min, const TGen::Vector3 & max);
		
		TGen::Vector3 center;
		scalar width, height, depth;
	};
	
	
} // !TGen

#endif // !_TGEN_AABB_H
