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

TGen::Quaternion4::Quaternion4(const TGen::Vector3 & axis, const TGen::Angle & angle) {
	scalar radianAngle = TGen::Radian(angle).angle * 0.5;
	scalar sinAngle = TGen::Sin(TGen::Radian(radianAngle));
	
	x = axis.x * sinAngle;
	y = axis.y * sinAngle;
	z = axis.z * sinAngle;
	w = TGen::Cos(TGen::Radian(radianAngle));
}

TGen::Quaternion4::Quaternion4()
	: x(0.0)
	, y(0.0)
	, z(0.0)
	, w(0.0)
{
}

TGen::Quaternion4::Quaternion4(const TGen::Vector3 & vector)
	: x(vector.x)
	, y(vector.y)
	, z(vector.z)
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
	return TGen::Matrix4x4(1.0 - (2.0 * y * y + 2.0 * z * z), 2.0 * x * y + 2.0 * z * w, 2.0 * x * z - 2.0 * y * w, 0,
						   2.0 * x * y - 2.0 * z * w, 1.0 - (2.0 * x * x + 2.0 * z * z), 2.0 * y * z + 2.0 * x * w, 0,
						   2.0 * x * z + 2.0 * y * w, 2.0 * y * z - 2.0 * x * w, 1.0 - (2.0 * x * x + 2.0 * y * y), 0,
						   0, 0, 0, 1.0);		
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
	return TGen::Quaternion4(*this).normalize();
}

TGen::Quaternion4 & TGen::Quaternion4::normalize() {
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

TGen::Quaternion4 TGen::Quaternion4::Slerp(const TGen::Quaternion4 & q1, const TGen::Quaternion4 & q2, scalar t) {
	scalar dot = TGen::Quaternion4::DotProduct(q1, q2);
				
	const scalar DOT_TRESHOLD = scalar(0.9995);
	if (dot > DOT_TRESHOLD) {
		TGen::Quaternion4 ret = q1 + (q2 - q1) * t;
		ret.normalize();
		return ret;
	}
	
	//Lead::Math::Clamp(dot, -1.0, 1.0);
	double theta_0 = ::acos(dot);
	double theta = theta_0 * t;
	
	Quaternion4 q3 = q2 - q1 * dot;
	q3.normalize();
	
	return q1 * static_cast<scalar>(::cos(theta)) + q3 * static_cast<scalar>(::sin(theta));
}

scalar TGen::Quaternion4::DotProduct(const TGen::Quaternion4 & q1, const TGen::Quaternion4 & q2) {
	return TGen::Vector3::DotProduct(TGen::Vector3(q1), TGen::Vector3(q2));
}

TGen::Quaternion4 TGen::Quaternion4::operator + (const TGen::Quaternion4 & quat) const {
	return TGen::Quaternion4(x + quat.x, y + quat.y, z + quat.z, w + quat.w);
}

TGen::Quaternion4 TGen::Quaternion4::operator - (const TGen::Quaternion4 & quat) const {
	return TGen::Quaternion4(x - quat.x, y - quat.y, z - quat.z, w - quat.w);	
}

TGen::Quaternion4 TGen::Quaternion4::operator * (scalar value) const {
	return TGen::Quaternion4(x * value, y * value, z * value, w * value);
}

TGen::Quaternion4 & TGen::Quaternion4::invert() {
	scalar length = (1.0 / (x * x + y * y + z * z + w * w));
	x *= -length;
	y *= -length;
	z *= -length;
	w *= length;
	
	return *this;
}

TGen::Quaternion4 & TGen::Quaternion4::getInverse() const {
	return TGen::Quaternion4(*this).invert();
}

TGen::Vector3 TGen::Quaternion4::operator * (const TGen::Vector3 & vector) const {
	TGen::Quaternion4 vectorQuat(vector), inverseQuat, resultQuat;
	TGen::Vector3 ret;
	
	inverseQuat = getInverse();
	resultQuat = vectorQuat * inverseQuat;
	resultQuat = *this * resultQuat;
	
	ret.x = resultQuat.x;
	ret.y = resultQuat.y;
	ret.z = resultQuat.z;
	
	return ret;
}

