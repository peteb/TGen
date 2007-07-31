/*
 *  quaternion4.cpp
 *  TGen Math
 *
 *  Created by Peter Backman on 7/30/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "quaternion4.h"
#include <cmath>
#include "angle.h"

TGen::Quaternion4::Quaternion4(scalar x, scalar y, scalar z, scalar w)
	: x(x)
	, y(y)
	, z(z)
	, w(w)
{		
}

TGen::Quaternion4::Quaternion4()
	: x(0.0)
	, y(0.0)
	, z(0.0)
	, w(0.0)
{
}

TGen::Quaternion4 & TGen::Quaternion4::operator *= (const TGen::Quaternion4 & quat) {
	x = w * quat.x + x * quat.w + y * quat.z - z * quat.y;
	y = w * quat.y - x * quat.z + y * quat.w + z * quat.x;
	z = w * quat.z + x * quat.y - y * quat.x + z * quat.w;
	w = w * quat.w - x * quat.x - y * quat.y - z * quat.z;		
	
	return *this;
}

TGen::Quaternion4 TGen::Quaternion4::operator * (const TGen::Quaternion4 & quat) const {
	return Quaternion4(w * quat.x + x * quat.w + y * quat.z - z * quat.y,
					   w * quat.y - x * quat.z + y * quat.w + z * quat.x,
					   w * quat.z + x * quat.y - y * quat.x + z * quat.w,
					   w * quat.w - x * quat.x - y * quat.y - z * quat.z);	
}

TGen::Quaternion4::operator TGen::Matrix4x4 () const {
	return TGen::Matrix4x4(1 - (2 * y * y + 2 * z * z), 2 * x * y + 2 * z * w, 2 * x * z - 2 * y * w, 0,
						   2 * x * y - 2 * z * w, 1 - (2 * x * x + 2 * z * z), 2 * y * z + 2 * x * w, 0,
						   2 * x * z + 2 * y * w, 2 * y * z - 2 * x * w, 1 - (2 * x * x + 2 * y * y), 0,
						   0, 0, 0, 1);		
}

TGen::Quaternion4::operator TGen::Vector3 () const {
	return TGen::Vector3(x, y, z);
}

TGen::Quaternion4::operator TGen::Vector4 () const {
	return TGen::Vector4(x, y, z, w);
}

scalar TGen::Quaternion4::getMagnitude() const {
	return static_cast<scalar>(sqrt(w * w + x * x + y * y + z * z));
}

TGen::Quaternion4 TGen::Quaternion4::getNormalized() const {
	return TGen::Quaternion4(*this).Normalize();
}

TGen::Quaternion4 & TGen::Quaternion4::Normalize() {
	scalar magnitude = getMagnitude();
	x /= magnitude;
	y /= magnitude;
	z /= magnitude;
	w /= magnitude;
	
	return *this;
}

TGen::Quaternion4 TGen::Quaternion4::operator - () const {
	return TGen::Quaternion4(-x, -y, -z, w);
}

TGen::Quaternion4 TGen::Quaternion4::Rotation(const TGen::Vector3 axis, const TGen::Angle & angle) {
	TGen::Quaternion4 ret;
	TGen::Radian halfAngle = TGen::Radian(angle) / 2.0;
	
	ret.x = axis.x * TGen::Sin(halfAngle);
	ret.y = axis.y * TGen::Sin(halfAngle);
	ret.z = axis.z * TGen::Sin(halfAngle);
	ret.w = TGen::Cos(halfAngle);
	
	return ret;
}
