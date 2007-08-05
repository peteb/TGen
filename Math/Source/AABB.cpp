/*
 *  AABB.cpp
 *  TGen Math
 *
 *  Created by Peter Backman on 7/18/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "AABB.h"

TGen::AABB::AABB() 
	: width(0.0f)
	, height(0.0f)
	, depth(0.0f)
{	
}

TGen::AABB::AABB(const TGen::Vector3 & center, scalar width, scalar height, scalar depth)
	: center(center)
	, width(width)
	, height(height)
	, depth(depth)
{
}

TGen::AABB::AABB(const TGen::Vector3 & min, const TGen::Vector3 & max)
	: width(0.0f)
	, height(0.0f)
	, depth(0.0f)
{
	calculate(min, max);
}

void TGen::AABB::calculate(const TGen::Vector3 & min, const TGen::Vector3 & max) {
	height = max.y - min.y;
	width = max.x - min.x;
	depth = max.z - min.z;
	center = TGen::Vector3(min.x + width / 2.0f, min.y + height / 2.0f, min.z + depth / 2.0f);
}

void TGen::AABB::getCorners(TGen::Vector3 * array) const {
	float halfWidth = width / 2.0f;
	float halfHeight = height / 2.0f;
	float halfDepth = depth / 2.0f;
	
	TGen::Vector3 min(center.x - halfWidth, center.y - halfHeight, center.z - halfDepth);
	TGen::Vector3 max(center.x + halfWidth, center.y + halfHeight, center.z + halfDepth);
	
	array[0] = min;
	array[1] = TGen::Vector3(max.x, min.y, min.z);
	array[2] = TGen::Vector3(max.x, min.y, max.z);
	array[3] = TGen::Vector3(min.x, min.y, max.z);
	array[4] = TGen::Vector3(min.x, max.y, min.z);
	array[5] = TGen::Vector3(max.x, max.y, min.z);
	array[6] = max;
	array[7] = TGen::Vector3(min.x, max.y, max.z);
}

