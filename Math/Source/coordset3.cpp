/*
 *  coordset3.cpp
 *  TGen Math
 *
 *  Created by Peter Backman on 9/10/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "coordset3.h"
#include "matrix4x4.h"
#include <sstream>

void TGen::CoordSet3::addCoord(const TGen::Vector3 & coord) {
	coords.push_back(coord);
}

TGen::CoordSet3 & TGen::CoordSet3::operator *= (const TGen::Matrix4x4 & mat) {
	for (int i = 0; i < coords.size(); ++i) {
		coords[i] = mat * coords[i];		
	}
	
	return *this;
}

TGen::CoordSet3::operator std::string() const {
	std::stringstream ss;
	for (int i = 0; i < coords.size(); ++i) {
		ss << i << ". " << std::string(coords[i]) << std::endl;
	}
	
	return ss.str();
}

int TGen::CoordSet3::size() const {
	return coords.size();
}

const TGen::Vector3 & TGen::CoordSet3::operator [] (int index) const {
	return coords[index];
}

TGen::CoordSet3 & TGen::CoordSet3::operator += (const TGen::CoordSet3 & set) {
	for (int i = 0; i < set.size(); ++i)
		addCoord(set[i]);
	
	return *this;
}

TGen::AABB TGen::CoordSet3::getBoundingBox() const {
	return TGen::AABB(getMin(), getMax());	
}

TGen::Vector3 TGen::CoordSet3::getMin() const {
	TGen::Vector3 min;
	bool first = true;
	
	for (int i = 0; i < coords.size(); ++i) {
		if (first) {
			min = coords[i];
			first = false;
		}
		else {
			min.x = std::min(min.x, coords[i].x);
			min.y = std::min(min.y, coords[i].y);
			min.z = std::min(min.z, coords[i].z);
		}
	}
	
	return min;
}

TGen::Vector3 TGen::CoordSet3::getMax() const {
	TGen::Vector3 max;
	bool first = true;
	
	for (int i = 0; i < coords.size(); ++i) {
		if (first) {
			max = coords[i];
			first = false;
		}
		else {
			max.x = std::max(max.x, coords[i].x);
			max.y = std::max(max.y, coords[i].y);
			max.z = std::max(max.z, coords[i].z);
		}
	}
	
	return max;
}

// TODO: replace getUpperLeft, getLowerRight with getMin, getMax in rectangle or wherever

TGen::Vector3 TGen::CoordSet3::getNormal(int index) const {
	int nextI = (index + 1) % coords.size();
	return TGen::Vector3::CrossProduct(coords[nextI], coords[index]);
}

TGen::Vector3 TGen::CoordSet3::getEdge(int index) const {
	int nextI = (index + 1) % coords.size();
	return coords[nextI] - coords[index];	
}

bool TGen::CoordSet3::intersects(const CoordSet3 & otherPolygon) const {
	scalar mina, maxa, minb, maxb;

	for (int i = 0; i < size(); ++i) {
		TGen::Vector3 axis = getNormal(i);
		axis.z = 0.0f;
		axis.normalize();
		
		projectOnAxis(axis, mina, maxa);
		otherPolygon.projectOnAxis(axis, minb, maxb);
		
		if (mina > maxb || minb > maxb)
			return false;
	}
	
	for (int i = 0; i < otherPolygon.size(); ++i) {
		TGen::Vector3 axis = otherPolygon.getNormal(i);
		axis.z = 0.0f;
		axis.normalize();
		
		projectOnAxis(axis, mina, maxa);
		otherPolygon.projectOnAxis(axis, minb, maxb);

		if (mina > maxb || minb > maxa)
			return false;		
	}
	
	return true;
}

void TGen::CoordSet3::projectOnAxis(const TGen::Vector3 & axis, scalar & min, scalar & max) const {
	min = max = TGen::Vector3::DotProduct(axis, coords[0]);
	
	for (int i = 1; i < size(); ++i) {
		scalar v = TGen::Vector3::DotProduct(coords[i], axis);
		
		if (v < min)
			min = v;
		else if (v > max)
			max = v;
	}
}


