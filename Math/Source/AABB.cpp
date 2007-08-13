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

TGen::Vector3 TGen::AABB::getMin() const {
	return center - TGen::Vector3(width / 2.0f, height / 2.0f, depth / 2.0f);
}

TGen::Vector3 TGen::AABB::getMax() const {
	return center + TGen::Vector3(width / 2.0f, height / 2.0f, depth / 2.0f);	
}


TGen::AABB & TGen::AABB::operator += (const TGen::AABB & box) {
	TGen::Vector3 min, max;
	TGen::Vector3 min1 = getMin();
	TGen::Vector3 max1 = getMax();
	TGen::Vector3 min2 = box.getMin();
	TGen::Vector3 max2 = box.getMax();
	
	min.x = std::min(min1.x, min2.x);
	min.y = std::min(min1.y, min2.y);
	min.z = std::min(min1.z, min2.z);
	
	max.x = std::max(max1.x, max2.x);
	max.y = std::max(max1.y, max2.y);
	max.z = std::max(max1.z, max2.z);

	calculate(min, max);
	
	return *this;
}

TGen::AABB TGen::AABB::operator + (const TGen::AABB & box) const {
	return TGen::AABB(*this) += box;
}
