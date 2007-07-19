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
	Calculate(min, max);
}

void TGen::AABB::Calculate(const TGen::Vector3 & min, const TGen::Vector3 & max) {
	height = max.y - min.y;
	width = max.x - min.x;
	depth = max.z - min.z;
	center = TGen::Vector3(min.x + width / 2.0f, min.y + height / 2.0f, min.z + depth / 2.0f);
}
