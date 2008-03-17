/*
 *  plane.cpp
 *  TGen Math
 *
 *  Created by Peter Backman on 7/19/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "plane3.h"

using TGen::scalar;

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
	//normal = TGen::Vector3::CrossProduct(p1 - p0, p2 - p0);
	//distance = (normal * p0).getMagnitude();	
	
	TGen::Vector3 v1, v2;
	v1 = p2 - p0;
	v2 = p1 - p0;
	normal = TGen::Vector3::CrossProduct(v1, v2).normalize();
	distance = -TGen::Vector3::DotProduct(normal, p2);
	/*
	 
	 TGen::Vector3 p1 = TGen::Vector3(-2.0f, 2.0f, 4.0f), p2 = TGen::Vector3(2.0f, 0.0f, 4.0f), p3 = TGen::Vector3(2.0f, 2.0f, 4.0f);
	 
	 TGen::Vector3 v1, v2, n;
	 v1 = p3 - p1;
	 v2 = p2 - p1;
	 n = TGen::Vector3::CrossProduct(v1, v2).normalize();
	 
	 planet.normal = n;
	 planet.distance = -(n.x * p3.x + n.y * p3.y + n.z * p3.z);

	 */
	
}

bool TGen::Plane3::isParallelTo(const TGen::Plane3 & plane) const {
	return (plane.normal == normal || plane.normal == -normal);	
}

scalar TGen::Plane3::getDistanceTo(const TGen::Vector3 & p) const {
	return TGen::Vector3::DotProduct(normal, p) + distance;	
}


TGen::Vector3 TGen::Plane3::rayIntersection(const TGen::Vector3 & pos, const TGen::Vector3 & dir) const {
	scalar a = TGen::Vector3::DotProduct(normal, dir);
	if (a == 0)
		return pos;
	
	return pos - dir * (getDistanceTo(pos) / a);
}

TGen::Vector3 TGen::Plane3::planeIntersection(const TGen::Plane3 & plane) const {
	return TGen::Vector3::CrossProduct(normal, plane.normal);	
}

TGen::Vector3 TGen::Plane3::project(const TGen::Vector3 & p) const {
	return TGen::Vector3(0, 0, 0);	// TODO
}

TGen::Plane3 & TGen::Plane3::normalize() {
	scalar mag = normal.getMagnitude();
	normal /= mag;
	distance /= mag;
	
	return *this;
}

TGen::Plane3 & TGen::Plane3::invert() {
	normal = -normal;
	distance = -distance;
	
	return *this;
}

scalar TGen::Plane3::getPointSide(const TGen::Vector3 & p) const {
	return TGen::Vector3::DotProduct(p, normal) - distance;	
}
