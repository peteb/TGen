/*
 *  coordset3.cpp
 *  TGen Math
 *
 *  Created by Peter Backman on 9/10/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "coordset2.h"
#include "matrix4x4.h"
#include "vector3.h"
#include <sstream>

void TGen::CoordSet2::addCoord(const TGen::Vector2 & coord) {
	coords.push_back(coord);
}

TGen::CoordSet2 & TGen::CoordSet2::operator *= (const TGen::Matrix4x4 & mat) {
	for (int i = 0; i < coords.size(); ++i) {
		coords[i] = mat * TGen::Vector3(coords[i]);		
	}
	
	return *this;
}

TGen::CoordSet2::operator std::string() const {
	std::stringstream ss;
	for (int i = 0; i < coords.size(); ++i) {
		ss << i << ". " << std::string(coords[i]) << std::endl;
	}
	
	return ss.str();
}

int TGen::CoordSet2::size() const {
	return coords.size();
}

const TGen::Vector2 & TGen::CoordSet2::operator [] (int index) const {
	return coords[index];
}

TGen::CoordSet2 & TGen::CoordSet2::operator += (const TGen::CoordSet2 & set) {
	for (int i = 0; i < set.size(); ++i)
		addCoord(set[i]);
	
	return *this;
}

TGen::Rectangle TGen::CoordSet2::getBoundingRect() const {
	return TGen::Rectangle(getMin(), getMax());
}


TGen::Vector2 TGen::CoordSet2::getMin() const {
	TGen::Vector2 min;
	bool first = true;
	
	for (int i = 0; i < coords.size(); ++i) {
		if (first) {
			min = coords[i];
			first = false;
		}
		else {
			min.x = std::min(min.x, coords[i].x);
			min.y = std::min(min.y, coords[i].y);
		}
	}
	
	return min;
}

TGen::Vector2 TGen::CoordSet2::getMax() const {
	TGen::Vector2 max;
	bool first = true;
	
	for (int i = 0; i < coords.size(); ++i) {
		if (first) {
			max = coords[i];
			first = false;
		}
		else {
			max.x = std::max(max.x, coords[i].x);
			max.y = std::max(max.y, coords[i].y);
		}
	}
	
	return max;
}

TGen::Vector2 TGen::CoordSet2::getNormal(int index) const {
	int nextI = (index + 1) % coords.size();
	return TGen::Vector2::CrossProduct(coords[nextI], coords[index]);
}

TGen::Vector2 TGen::CoordSet2::getEdge(int index) const {
	int nextI = (index + 1) % coords.size();
	return coords[nextI] - coords[index];	
}

bool TGen::CoordSet2::intersects(const CoordSet2 & otherPolygon) const {
	scalar mina, maxa, minb, maxb;

	for (int i = 0; i < size(); ++i) {
		TGen::Vector2 axis = getNormal(i);
//		axis.z = 0.0f;
	//	axis.normalize();
		
		projectOnAxis(axis, mina, maxa);
		otherPolygon.projectOnAxis(axis, minb, maxb);
		
		if (mina > maxb || minb > maxb)
			return false;
	}
	
	for (int i = 0; i < otherPolygon.size(); ++i) {
		TGen::Vector2 axis = otherPolygon.getNormal(i);
		//axis.z = 0.0f;
		//axis.normalize();
		
		projectOnAxis(axis, mina, maxa);
		otherPolygon.projectOnAxis(axis, minb, maxb);

		if (mina > maxb || minb > maxa)
			return false;		
	}
	
	return true;
}

void TGen::CoordSet2::projectOnAxis(const TGen::Vector2 & axis, scalar & min, scalar & max) const {
	min = max = TGen::Vector2::DotProduct(axis, coords[0]);
	
	for (int i = 1; i < size(); ++i) {
		scalar v = TGen::Vector2::DotProduct(coords[i], axis);
		
		if (v < min)
			min = v;
		else if (v > max)
			max = v;
	}
}


