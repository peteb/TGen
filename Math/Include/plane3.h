/*
 *  plane3.h
 *  TGen Math
 *
 *  Created by Peter Backman on 7/19/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_PLANE3_H
#define _TGEN_PLANE3_H

#include "vector3.h"

namespace TGen {
	class Plane3 {
	public:
		Plane3();
		Plane3(const TGen::Vector3 & n, scalar d);
		Plane3(const TGen::Vector3 & p0, const TGen::Vector3 & p1, const TGen::Vector3 & p2);
		
		bool isParallelTo(const TGen::Plane3 & plane) const;
		scalar getDistanceTo(const TGen::Vector3 & p) const;
		TGen::Vector3 RayIntersection(const TGen::Vector3 & pos, const TGen::Vector3 & dir) const;
		TGen::Vector3 PlaneIntersection(const TGen::Plane3 & plane) const;
		TGen::Vector3 Project(const TGen::Vector3 & p) const;
		scalar getPointSide(const TGen::Vector3 & p) const;
		void Normalize();
		
		TGen::Vector3 normal;
		scalar distance;
	};	
	
} // !TGen

#endif // !_TGEN_PLANE3_H
