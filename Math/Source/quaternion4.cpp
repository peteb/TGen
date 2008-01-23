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

TGen::Quaternion4::Quaternion4(scalar x, scalar y, scalar z)
	: x(x)
	, y(y)
	, z(z)
{
	w = calculateW();
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
	*this = *this * quat;
	
	return *this;
}

TGen::Quaternion4 TGen::Quaternion4::operator * (const TGen::Quaternion4 & quat) const {
	return TGen::Quaternion4(x * quat.w + w * quat.x + y * quat.z - z * quat.y,
									 y * quat.w + w * quat.y + z * quat.x - x * quat.z,
									 z * quat.w + w * quat.z + x * quat.y - y * quat.x,
									 w * quat.w - x * quat.x - y * quat.y - z * quat.z);
}

TGen::Quaternion4::operator TGen::Matrix4x4 () const {
	return TGen::Matrix4x4(1.0 - (2.0 * y * y + 2.0 * z * z), 2.0 * x * y + 2.0 * z * w, 2.0 * x * z - 2.0 * y * w, 0,
						   2.0 * x * y - 2.0 * z * w, 1.0 - (2.0 * x * x + 2.0 * z * z), 2.0 * y * z + 2.0 * x * w, 0,
						   2.0 * x * z + 2.0 * y * w, 2.0 * y * z - 2.0 * x * w, 1.0 - (2.0 * x * x + 2.0 * y * y), 0,
						   0, 0, 0, 1.0);		
}

//TGen::Quaternion4::operator TGen::Vector3 () const {
//	return TGen::Vector3(x, y, z);
//}

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
	
	if (magnitude > 0.0f) {
		scalar inMag = 1.0f / magnitude;
		x *= inMag;
		y *= inMag;
		z *= inMag;
		w *= inMag;
	}
	
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

TGen::Quaternion4 TGen::Quaternion4::operator * (const TGen::Vector3 & vector) const {
	return TGen::Quaternion4(w * vector.x + y * vector.z - z * vector.y,
									 w * vector.y + z * vector.x - x * vector.z,
									 w * vector.z + x * vector.y - y * vector.x,
									 -(x * vector.x - y * vector.y - z * vector.z));
}

TGen::Vector3 TGen::Quaternion4::rotatePoint(const TGen::Vector3 & point) const {
	TGen::Quaternion4 temp, inverse;
	inverse.normalize();
	
	temp = *this * point;
	TGen::Quaternion4 ret = temp * inverse;
	
	return TGen::Vector3(ret.x, ret.y, ret.z);
}

scalar TGen::Quaternion4::calculateW() const {
	float t = 1.0f - x * x - y * y - z * z;
	
	if (t < 0.0f)
		return 0.0f;
	
	return -sqrt(t);
}

