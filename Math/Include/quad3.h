/*
 *  quad3.h
 *  TGen Math
 *
 *  Created by Peter Backman on 9/10/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_QUAD3_H
#define _TGEN_QUAD3_H

#include "vector3.h"
#include "AABB.h"

namespace TGen {
	class Quad3 {
	public:	
		Quad3();
		Quad3(const TGen::Vector3 & p1, const TGen::Vector3 & p2, const TGen::Vector3 & p3, const TGen::Vector3 & p4);
		Quad3(const TGen::Vector3 * points);
		
		operator TGen::AABB() const;
		
		TGen::Vector3 getMax() const;
		TGen::Vector3 getMin() const;
		
		TGen::Vector3 points[4];
	};
	
	
	
} // !TGen

#endif // !_TGEN_QUAD3_H

