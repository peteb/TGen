/*
 *  frustum.cpp
 *  TGen Math
 *
 *  Created by Peter Backman on 7/20/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "frustum.h"
#include "matrix4x4.h"
#include "vector3.h"

#include <iostream>

TGen::Frustum::Frustum(const TGen::Matrix4x4 & projection, const TGen::Matrix4x4 & worldview) {
	calculate(projection, worldview);
}


void TGen::Frustum::calculate(const TGen::Matrix4x4 & projection, const TGen::Matrix4x4 & worldview) {
	TGen::Matrix4x4 combined = projection * worldview;
	
	rightPlane.normal = combined.getOrigin() - combined.getX();
	rightPlane.distance = combined.elements[3][3] - combined.elements[0][3];
	
	leftPlane.normal = combined.getOrigin() + combined.getX();
	leftPlane.distance = combined.elements[3][3] + combined.elements[0][3];
	
	bottomPlane.normal = combined.getOrigin() + combined.getY();
	bottomPlane.distance = combined.elements[3][3] + combined.elements[1][3];
	
	topPlane.normal = combined.getOrigin() - combined.getY();
	topPlane.distance = combined.elements[3][3] - combined.elements[3][1];
	
	farPlane.normal = combined.getOrigin() - combined.getZ();
	farPlane.distance = combined.elements[3][3] - combined.elements[2][3];
	
	nearPlane.normal = combined.getOrigin() + combined.getZ();
	nearPlane.distance = combined.elements[3][3] + combined.elements[2][3];
	nearPlane.normalize();
	std::cout << "DIST::::::::::: " << nearPlane.distance << std::endl;
	
	
	rightPlane.normalize();
	leftPlane.normalize();
	bottomPlane.normalize();
	topPlane.normalize();
	nearPlane.normalize();
	farPlane.normalize();
	
	//farPlane.normal.normalize();
}

int TGen::Frustum::intersects(const TGen::Sphere & sphere) const {
	return 0;
}

int TGen::Frustum::intersects(const TGen::AABB & aabb) const {

	return 0;
}

int TGen::Frustum::intersects(const TGen::Vector3 & point) const {
	std::cout << "FAR: " << std::string(farPlane.normal) << " - " << farPlane.distance << std::endl;
	
	if (nearPlane.getDistanceTo(point) >= 0.0f && farPlane.getDistanceTo(point) >= 0.0f && rightPlane.getDistanceTo(point) >= 0.0f && leftPlane.getDistanceTo(point) >= 0.0f
		&& topPlane.getDistanceTo(point) >= 0.0f && bottomPlane.getDistanceTo(point) >= 0.0f)
		return 0;
	
	return 1;
}

