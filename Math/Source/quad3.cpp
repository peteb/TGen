/*
 *  quad3.cpp
 *  TGen Math
 *
 *  Created by Peter Backman on 9/10/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "quad3.h"

TGen::Quad3::Quad3() {
	
}

TGen::Quad3::Quad3(const TGen::Vector3 & p1, const TGen::Vector3 & p2, const TGen::Vector3 & p3, const TGen::Vector3 & p4) {
	points[0] = p1;
	points[1] = p2;
	points[2] = p3;
	points[3] = p4;
}

TGen::Quad3::Quad3(const TGen::Vector3 * points) {
	for (int i = 0; i < 4; ++i)
		this->points[i] = points[i];
	
}


TGen::Quad3::operator TGen::AABB() const {
	return TGen::AABB(getMin(), getMax());
}

TGen::Vector3 TGen::Quad3::getMax() const {
	TGen::Vector3 max;
	
	for (int i = 0; i < 4; ++i) {
		if (i == 0) {
			max = points[i];
		}
		else {
			max.x = std::max(points[i].x, max.x);
			max.y = std::max(points[i].y, max.y);
			max.z = std::max(points[i].z, max.z);
		}
	}
	
	return max;
}

TGen::Vector3 TGen::Quad3::getMin() const {
	TGen::Vector3 min;
	
	for (int i = 0; i < 4; ++i) {
		if (i == 0) {
			min = points[i];
		}
		else {
			min.x = std::max(points[i].x, min.x);
			min.y = std::max(points[i].y, min.y);
			min.z = std::max(points[i].z, min.z);
		}
	}	
	
	return min;
}

