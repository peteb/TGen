/*
 *  vector2.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/6/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "vector2.h"
#include "vector3.h"
#include <cmath>
#include <sstream>

using TGen::scalar;

TGen::Vector2 TGen::Vector2::Identity(1.0f, 1.0f);
TGen::Vector2 TGen::Vector2::Zero(0.0f, 0.0f);

TGen::Vector2::Vector2(const TGen::Vector2 & vector) throw() 
	: x(vector.x)
	, y(vector.y) 
{
}

TGen::Vector2::Vector2(const TGen::Vector3 & vector) throw() 
	: x(vector.x)
	, y(vector.y) 
{
}

TGen::Vector2::Vector2(scalar x, scalar y) throw() 
	: x(x)
	, y(y) 
{
}

TGen::Vector2::Vector2(scalar scalar) throw() 
	: x(scalar)
	, y(scalar) 
{
}

TGen::Vector2 & TGen::Vector2::operator += (const TGen::Vector2 & vector) throw() {
	x += vector.x;
	y += vector.y;
	
	return *this;
}

TGen::Vector2 & TGen::Vector2::operator -= (const TGen::Vector2 & vector) throw() {
	x -= vector.x;
	y -= vector.y;
	
	return *this;	
}

TGen::Vector2 & TGen::Vector2::operator = (const TGen::Vector2 & vector) throw() {
	x = vector.x;
	y = vector.y;
	
	return *this;
}

TGen::Vector2 & TGen::Vector2::operator *= (scalar scalar) throw() {
	x *= scalar;
	y *= scalar;
	
	return *this;
}

TGen::Vector2 TGen::Vector2::operator + (const TGen::Vector2 & vector) const throw() {
	return TGen::Vector2(x + vector.x, y + vector.y);
}

TGen::Vector2 TGen::Vector2::operator - (const TGen::Vector2 & vector) const throw() {
	return TGen::Vector2(x - vector.x, y - vector.y);	
}

TGen::Vector2 TGen::Vector2::operator * (scalar _scalar) const throw() {
	return TGen::Vector2(x * _scalar, y * _scalar);
}

TGen::Vector2 & TGen::Vector2::operator *= (const TGen::Vector2 & vector) throw() {
	x *= vector.x;
	y *= vector.y;
	
	return *this;
}

TGen::Vector2 TGen::Vector2::operator * (const TGen::Vector2 & vector) const throw() {
	return TGen::Vector2(x * vector.x, y * vector.y);
}

TGen::Vector2 TGen::Vector2::operator / (scalar scalar) const throw() {
	return TGen::Vector2(x / scalar, y / scalar);
}

TGen::Vector2 TGen::Vector2::operator - () const throw() {
	return TGen::Vector2(-x, -y);
}

scalar TGen::Vector2::getMagnitude() const throw() {
	return fabs(sqrt(x * x + y * y));
}

TGen::Vector2 TGen::Vector2::getNormalized() const throw() {
	return TGen::Vector2(*this).normalize();
}

TGen::Vector2 & TGen::Vector2::normalize() throw() {
	scalar magnitude = getMagnitude();
	x /= magnitude;
	y /= magnitude;

	return *this;
}

scalar TGen::Vector2::DotProduct(const TGen::Vector2 & v1, const TGen::Vector2 & v2) throw() {
	return v1.x * v2.x + v1.y * v2.y; // + v1.z * v2.z;
}

TGen::Vector2 TGen::Vector2::CrossProduct(const TGen::Vector2 & v1, const TGen::Vector2 & v2) throw() {
	/*	return TGen::Vector(v1.y * v2.z - v2.y * v1.z,  
	v1.z * v2.x - v2.z * v1.x,  
	v1.x * v2.y - v2.x * v1.y);  	
*/
	
	return TGen::Vector2(v1.y - v2.y,  
				      v2.x - v1.x);  	
}

TGen::Vector2 TGen::Vector2::getPerpendicular() const throw() {
	return TGen::Vector2(y, -x);
}

TGen::Vector2::operator std::string() const {
	std::stringstream ss;
	ss << "(" << x << ", " << y << ")";
	
	return ss.str();
}

TGen::Vector2 TGen::Vector2::Parse(const std::string & text) {
	if (text.find("identity") != std::string::npos)
		return TGen::Vector2::Identity;
	
	TGen::Vector2 ret;
	std::stringstream ss;
	ss << text;
	ss >> ret.x >> ret.y;
	
	return ret;	
}

