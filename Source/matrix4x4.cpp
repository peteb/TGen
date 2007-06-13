/*
 *  matrix4x4.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/6/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "matrix4x4.h"
#include "matrix3x3.h"
#include "vector3.h"
#include "vector2.h"
#include "rectangle.h"

#include <sstream>

TGen::Matrix4x4 TGen::Matrix4x4::Identity(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

TGen::Matrix4x4::Matrix4x4() {
	Clear();
}

TGen::Matrix4x4::Matrix4x4(const TGen::Vector3 & x, const TGen::Vector3 & y, const TGen::Vector3 & z) {
	Clear();
}

TGen::Matrix4x4::Matrix4x4(const TGen::Matrix4x4 & matrix) {
	memcpy(&elements, &matrix.elements, 4 * 4 * sizeof(scalar));	
}

TGen::Matrix4x4::Matrix4x4(const TGen::Matrix3x3 & matrix) {
	Clear();
	elements[0][0] = matrix.elements[0][0];
	elements[0][1] = matrix.elements[0][1];
	elements[0][3] = matrix.elements[0][2];
	
	elements[1][0] = matrix.elements[1][0];
	elements[1][1] = matrix.elements[1][1];
	elements[1][3] = matrix.elements[1][2];
	
	elements[2][2] = 1.0f;	// Z = (0, 0, 1)
	elements[2][3] = 0.0f; //matrix.elements[2][2];
	
	elements[3][0] = matrix.elements[2][0];
	elements[3][1] = matrix.elements[2][1];
	elements[3][2] = 0.0f;	// origin z
	elements[3][3] = matrix.elements[2][2];
}

TGen::Matrix4x4::Matrix4x4(scalar e11, scalar e12, scalar e13, scalar e14, scalar e21, scalar e22, scalar e23, scalar e24, scalar e31, scalar e32, scalar e33, scalar e34, scalar e41, scalar e42, scalar e43, scalar e44) {
	elements[0][0] = e11;
	elements[0][1] = e12;
	elements[0][2] = e13;
	elements[0][3] = e14;
	elements[1][0] = e21;
	elements[1][1] = e22;
	elements[1][2] = e23;
	elements[1][3] = e24;
	elements[2][0] = e31;
	elements[2][1] = e32;
	elements[2][2] = e33;
	elements[2][3] = e34;
	elements[3][0] = e41;
	elements[3][1] = e42;
	elements[3][2] = e43;
	elements[3][3] = e44;
}

TGen::Matrix4x4 & TGen::Matrix4x4::operator = (const TGen::Matrix4x4 & matrix) {
	memcpy(&elements, &matrix.elements, 4 * 4 * sizeof(scalar));	
	
	return *this;
}

TGen::Vector3 TGen::Matrix4x4::operator * (const TGen::Vector3 & vector) const {
	TGen::Vector3 ret;
	ret.x = vector.x * elements[0][0] + vector.y * elements[1][0] + vector.z * elements[2][0] + elements[3][0];
	ret.y = vector.x * elements[0][1] + vector.y * elements[1][1] + vector.z * elements[2][1] + elements[3][1];
	ret.z = vector.x * elements[0][2] + vector.y * elements[1][2] + vector.z * elements[2][2] + elements[3][2];
	
	return TGen::Vector3::Identity;
}

TGen::Matrix4x4 TGen::Matrix4x4::operator * (const TGen::Matrix4x4 & matrix) const {
	
	return *this;
}

TGen::Matrix4x4::operator std::string() const {
	std::stringstream ss;
	
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			ss << "[" << elements[x][y] << "]";
		}
		
		ss << "\n";
	}
	
	return ss.str();	
}

void TGen::Matrix4x4::setOrigin(const TGen::Vector3 & origin) {
	elements[3][0] = origin.x;
	elements[3][1] = origin.y;
	elements[3][2] = origin.z;
}

void TGen::Matrix4x4::setScale(const TGen::Vector3 & scale) {
	elements[0][0] = scale.x;
	elements[1][1] = scale.y;
	elements[2][2] = scale.z;
}

void TGen::Matrix4x4::get4x4(scalar * elements) const {
	
}


TGen::Matrix4x4 TGen::Matrix4x4::Translation(const TGen::Vector3 & vector) {
	TGen::Matrix4x4 matrix = Identity;
	matrix.setOrigin(vector);
	return matrix;
}

TGen::Matrix4x4 TGen::Matrix4x4::Scaling(const TGen::Vector3 & scale) {
	TGen::Matrix4x4 matrix = Identity;
	matrix.setScale(scale);
	return matrix;
}

TGen::Matrix4x4 TGen::Matrix4x4::OrthogonalProjection(scalar left, scalar top, scalar right, scalar bottom, scalar near, scalar far) {
	Matrix4x4 ret;
	
	ret.elements[0][0] = 2.0 / (right - left);
	ret.elements[1][1] = 2.0 / (top - bottom);
	ret.elements[2][2] = 0.0; //(T)-2.0 / (far - near);
	ret.elements[3][3] = 1;
	ret.elements[3][0] = - (right + left) / (right - left);
	ret.elements[3][1] = - (top + bottom) / (top - bottom);
	ret.elements[3][2] = 0.0; //- (far + near) / (far - near);
	
	return ret;
}

TGen::Matrix4x4 TGen::Matrix4x4::OrthogonalProjection(const TGen::Rectangle & area, scalar near, scalar far) {
	TGen::Vector2 upperLeft = area.getUpperLeft();

	return OrthogonalProjection(upperLeft.x, upperLeft.y, upperLeft.x + area.width, upperLeft.y + area.height, near, far);
}

void TGen::Matrix4x4::Clear() {
	memset(&elements, 0, 4 * 4 * sizeof(scalar));
}

