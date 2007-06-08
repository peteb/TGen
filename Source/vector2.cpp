/*
 *  vector2.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/6/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "vector2.h"
#include <cmath>
#include <sstream>

TGen::Vector2::Vector2(const TGen::Vector2 & vector) : x(vector.x), y(vector.y)/*, z(vector.z)*/ {}
TGen::Vector2::Vector2(float x, float y/*, float z*/) : x(x), y(y)/*, z(z)*/ {}
TGen::Vector2::Vector2(float scalar) : x(scalar), y(scalar)/*, z(scalar)*/ {}

TGen::Vector2 & TGen::Vector2::operator += (const TGen::Vector2 & vector) {
	x += vector.x;
	y += vector.y;
	//	z += vector.z;
	
	return *this;
}

TGen::Vector2 & TGen::Vector2::operator -= (const TGen::Vector2 & vector) {
	x -= vector.x;
	y -= vector.y;
	//	z -= vector.z;
	
	return *this;	
}

TGen::Vector2 & TGen::Vector2::operator = (const TGen::Vector2 & vector) {
	x = vector.x;
	y = vector.y;
	//	z = vector.z;
	
	return *this;
}

TGen::Vector2 & TGen::Vector2::operator *= (float scalar) {
	x *= scalar;
	y *= scalar;
	//	z *= scalar;
	
	return *this;
}

TGen::Vector2 TGen::Vector2::operator + (const TGen::Vector2 & vector) const {
	return TGen::Vector2(x + vector.x, y + vector.y/*, z + vector.z*/);
}

TGen::Vector2 TGen::Vector2::operator - (const TGen::Vector2 & vector) const {
	return TGen::Vector2(x - vector.x, y - vector.y/*, z - vector.z*/);	
}

TGen::Vector2 TGen::Vector2::operator * (float scalar) const {
	return TGen::Vector2(x * scalar, y * scalar/*, z * scalar*/);
}

TGen::Vector2 TGen::Vector2::operator / (float scalar) const {
	return TGen::Vector2(x / scalar, y / scalar/*, z / scalar*/);
}

TGen::Vector2 TGen::Vector2::operator - () const {
	return TGen::Vector2(-x, -y/*, -z*/);
}

float TGen::Vector2::getMagnitude() const {
	return fabs(sqrt(x * x + y * y /* + z * z*/));
}

TGen::Vector2 TGen::Vector2::getNormalized() const {
	return TGen::Vector2(*this).Normalize();
}

TGen::Vector2 & TGen::Vector2::Normalize() {
	float magnitude = getMagnitude();
	x /= magnitude;
	y /= magnitude;
	//	z /= magnitude;
	
	return *this;
}

float TGen::Vector2::DotProduct(const TGen::Vector2 & v1, const TGen::Vector2 & v2) {
	return v1.x * v2.x + v1.y * v2.y; // + v1.z * v2.z;
}

TGen::Vector2 TGen::Vector2::CrossProduct(const TGen::Vector2 & v1, const TGen::Vector2 & v2) {
	/*	return TGen::Vector(v1.y * v2.z - v2.y * v1.z,  
	v1.z * v2.x - v2.z * v1.x,  
	v1.x * v2.y - v2.x * v1.y);  	
*/
	
	return TGen::Vector2(v1.y - v2.y,  
				      v2.x - v1.x);  	
}

TGen::Vector2 TGen::Vector2::getPerpendicular() const {
	return TGen::Vector2(y, -x);
}

TGen::Vector2::operator std::string() const {
	std::stringstream ss;
	ss << "(" << x << ", " << y << ")";
	
	return ss.str();
}


