/*
 *  vector3.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/6/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "vector3.h"
#include "vector4.h"
#include "vector2.h"
#include <sstream>
#include <cmath>
//#include <tgen_core.h>

using TGen::scalar;

TGen::Vector3 TGen::Vector3::Identity(1.0f, 1.0f, 1.0f);
TGen::Vector3 TGen::Vector3::Zero(0.0f, 0.0f, 0.0f);

/*TGen::Vector3::Vector3(const TGen::Vector4 & vector) 
	: x(vector.x)
	, y(vector.y)
	, z(vector.z)
{
}*/

/*TGen::Vector3::Vector3(const TGen::Vector3 & vector) 
	: x(vector.x)
	, y(vector.y)
	, z(vector.z) 
{
}

TGen::Vector3::Vector3(const TGen::Vector2 & vector) 
	: x(vector.x)
	, y(vector.y)
	, z(0.0f) 
{
}
*/
/*TGen::Vector3::Vector3(scalar x, scalar y, scalar z)
	: x(x)
	, y(y)
	, z(z) 
{
}

TGen::Vector3::Vector3(scalar value)
	: x(value)
	, y(value)
	, z(value) 
{
}
*/
/*TGen::Vector3::Vector3() 
	: x(0.0f)
	, y(0.0f)
	, z(0.0f)
{
}*/

TGen::Vector3::operator std::string() const {
	std::stringstream ss;
	ss << "(" << x << ", " << y << ", " << z << ")";
	
	return ss.str();
}

scalar TGen::Vector3::DotProduct(const TGen::Vector3 & v1, const TGen::Vector3 & v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

TGen::Vector3 TGen::Vector3::CrossProduct(const TGen::Vector3 & v1, const TGen::Vector3 & v2) {
	return TGen::Vector3(v1.y * v2.z - v2.y * v1.z,  
						 v1.z * v2.x - v2.z * v1.x,  
						 v1.x * v2.y - v2.x * v1.y);  	
}

TGen::Vector3 & TGen::Vector3::operator += (const TGen::Vector3 & vector) {
	x += vector.x;
	y += vector.y;
	z += vector.z;
	
	return *this;
}

TGen::Vector3 & TGen::Vector3::operator -= (const TGen::Vector3 & vector) {
	x -= vector.x;
	y -= vector.y;
	z -= vector.z;
	
	return *this;
}

TGen::Vector3 & TGen::Vector3::operator /= (scalar scal) {
	x /= scal;
	y /= scal;
	z /= scal;
	
	return *this;
}

TGen::Vector3 & TGen::Vector3::operator = (const TGen::Vector3 & vector) {
	x = vector.x;
	y = vector.y;
	z = vector.z;
	
	return *this;
}

TGen::Vector3 & TGen::Vector3::operator *= (scalar scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;
	
	return *this;
}

bool TGen::Vector3::operator == (const TGen::Vector3 & vector) const {
	return (x == vector.x && y == vector.y && z == vector.z);
}

TGen::Vector3 TGen::Vector3::operator + (const TGen::Vector3 & vector) const {
	return TGen::Vector3(x + vector.x, y + vector.y, z + vector.z);
}

TGen::Vector3 TGen::Vector3::operator - (const TGen::Vector3 & vector) const {
	return TGen::Vector3(x - vector.x, y - vector.y, z - vector.z);
}

TGen::Vector3 TGen::Vector3::operator * (scalar scalar) const {
	return TGen::Vector3(x * scalar, y * scalar, z * scalar);
}

TGen::Vector3 TGen::Vector3::operator * (const TGen::Vector3 & vector) const {
	return TGen::Vector3(x * vector.x, y * vector.y, z * vector.z);
}

TGen::Vector3 TGen::Vector3::operator / (scalar scalar) const {
	return TGen::Vector3(x / scalar, y / scalar, z / scalar);
}

TGen::Vector3 TGen::Vector3::operator - () const {
	return TGen::Vector3(-x, -y, -z);
}

bool TGen::Vector3::operator < (const TGen::Vector3 & vector) const {
	return (x < vector.x && y < vector.y);
}

bool TGen::Vector3::operator > (const TGen::Vector3 & vector) const {
	return (x > vector.x && y > vector.y);
}

TGen::Vector3 TGen::Vector3::Parse(const std::string & text) {
	if (text.find("identity") != std::string::npos || text.find("ident") != std::string::npos)
		return TGen::Vector3::Identity;
	else if (text.find("zero") != std::string::npos)
		return TGen::Vector3::Zero;
	
	TGen::Vector3 ret;
	std::stringstream ss;
	ss << text;
	ss >> ret.x >> ret.y >> ret.z;
	
	return ret;
}

