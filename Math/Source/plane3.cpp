/*
 *  plane.cpp
 *  TGen Math
 *
 *  Created by Peter Backman on 7/19/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "plane3.h"

TGen::Plane3::Plane3()
	: distance(0.0f)
{		
}

TGen::Plane3::Plane3(const TGen::Vector3 & n, scalar d) 
	: normal(n)
	, distance(d)
{
}

// TODO: test plane code

TGen::Plane3::Plane3(const TGen::Vector3 & p0, const TGen::Vector3 & p1, const TGen::Vector3 & p2) {
	normal = TGen::Vector3::CrossProduct(p1 - p0, p2 - p0);
	distance = (normal * p0).getMagnitude();	
}

bool TGen::Plane3::isParallelTo(const TGen::Plane3 & plane) const {
	return (plane.normal == normal || plane.normal == -normal);	
}

scalar TGen::Plane3::getDistanceTo(const TGen::Vector3 & p) const {
	return TGen::Vector3::DotProduct(normal, p) + distance;	
}


TGen::Vector3 TGen::Plane3::RayIntersection(const TGen::Vector3 & pos, const TGen::Vector3 & dir) const {
	scalar a = TGen::Vector3::DotProduct(normal, dir);
	if (a == 0)
		return pos;
	
	return pos - dir * (getDistanceTo(pos) / a);
}

TGen::Vector3 TGen::Plane3::PlaneIntersection(const TGen::Plane3 & plane) const {
	return TGen::Vector3::CrossProduct(normal, plane.normal);	
}

TGen::Vector3 TGen::Plane3::Project(const TGen::Vector3 & p) const {
	return TGen::Vector3(0, 0, 0);	// TODO
}

void TGen::Plane3::Normalize() {
	normal.Normalize();
}

scalar TGen::Plane3::getPointSide(const TGen::Vector3 & p) const {
	return TGen::Vector3::DotProduct(p, normal) - distance;	
}
