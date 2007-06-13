/*
 *  Matrix3x33x3.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/6/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "matrix3x3.h"
#include "matrix4x4.h"
#include "Vector2.h"
#include <cstring>
#include <sstream>

/*
	XY
	xx
	yy
 */

TGen::Matrix3x3::Matrix3x3() {
	Clear();
}

TGen::Matrix3x3 TGen::Matrix3x3::Identity(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

TGen::Matrix3x3::Matrix3x3(const TGen::Vector2 & x, const TGen::Vector2 & y) {
	Clear();
	elements[0][0] = x.x;
	elements[0][1] = x.y;
	elements[1][0] = y.x;
	elements[1][1] = y.y;
	elements[2][2] = 1.0f;
}

TGen::Matrix3x3::Matrix3x3(const TGen::Matrix3x3 & matrix) {
	memcpy(&elements, &matrix.elements, 3 * 3 * sizeof(scalar));
}

TGen::Matrix3x3::Matrix3x3(const TGen::Matrix4x4 & matrix) {
	Clear();
	
	elements[0][0] = matrix.elements[0][0];
	elements[0][1] = matrix.elements[0][1];
	elements[0][2] = matrix.elements[0][3];
	
	elements[1][0] = matrix.elements[1][0];
	elements[1][1] = matrix.elements[1][1];
	elements[1][2] = matrix.elements[1][3];
	
	elements[2][0] = matrix.elements[3][0];
	elements[2][1] = matrix.elements[3][1];
	elements[2][2] = matrix.elements[3][3];
}

TGen::Matrix3x3::Matrix3x3(scalar e11, scalar e12, scalar e13, scalar e21, scalar e22, scalar e23, scalar e31, scalar e32, scalar e33) {
	elements[0][0] = e11;
	elements[0][1] = e12;
	elements[0][2] = e13;
	elements[1][0] = e21;
	elements[1][1] = e22;
	elements[1][2] = e23;
	elements[2][0] = e31;
	elements[2][1] = e32;
	elements[2][2] = e33;
}

void TGen::Matrix3x3::Clear() {
	memset(&elements, 0, 3 * 3 * sizeof(scalar));
}

TGen::Matrix3x3 & TGen::Matrix3x3::operator = (const TGen::Matrix3x3 & matrix) {
	memcpy(elements, matrix.elements, sizeof(scalar) * 3 * 3);
	return *this;
}

TGen::Vector2 TGen::Matrix3x3::operator * (const TGen::Vector2 & Vector2) const {
	TGen::Vector2 ret;
	ret.x = Vector2.x * elements[0][0] + Vector2.y * elements[1][0] + elements[2][0];
	ret.y = Vector2.x * elements[0][1] + Vector2.y * elements[1][1] + elements[2][1];
	
	return ret;
}

TGen::Matrix3x3 TGen::Matrix3x3::operator * (const TGen::Matrix3x3 & Matrix3x3) const {
	TGen::Matrix3x3 ret = TGen::Matrix3x3::Identity;
	
	int i, j, k;
	
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			for (k = 0, ret.elements[i][j] = 0; k < 3; k++)
				ret.elements[i][j] += elements[i][k] * Matrix3x3.elements[k][j];	
		}
	}
	
	
	return ret;	
}

void TGen::Matrix3x3::setOrigin(const TGen::Vector2 & origin) {
	elements[2][0] = origin.x;
	elements[2][1] = origin.y;
}

void TGen::Matrix3x3::get4x4(scalar * elements) const {
	//		 x   y
	elements[0 + 0 * 4] = this->elements[0][0];
	elements[0 + 1 * 4] = this->elements[1][0];
	elements[0 + 2 * 4] = 0.0f;
	elements[0 + 3 * 4] = this->elements[2][0];
	
	elements[1 + 0 * 4] = this->elements[0][1];
	elements[1 + 1 * 4] = this->elements[1][1];
	elements[1 + 2 * 4] = 0.0f;
	elements[1 + 3 * 4] = this->elements[2][1];
	
	elements[2 + 0 * 4] = 0.0f;
	elements[2 + 1 * 4] = 0.0f;
	elements[2 + 2 * 4] = 1.0f;
	elements[2 + 3 * 4] = 0.0f;
	
	elements[3 + 0 * 4] = this->elements[0][2];
	elements[3 + 1 * 4] = this->elements[1][2];
	elements[3 + 2 * 4] = 0.0f;
	elements[3 + 3 * 4] = this->elements[2][2];
	
	
	
}

void TGen::Matrix3x3::setScale(const TGen::Vector2 & scale) {
	elements[0][0] = scale.x;
	elements[1][1] = scale.y;
	
}

TGen::Matrix3x3 TGen::Matrix3x3::Translation(const TGen::Vector2 & vector) {
	TGen::Matrix3x3 matrix = Identity;
	matrix.setOrigin(vector);
	return matrix;
}

TGen::Matrix3x3 TGen::Matrix3x3::Scaling(const TGen::Vector2 & scale) {
	TGen::Matrix3x3 matrix = Identity;
	matrix.setScale(scale);
	return matrix;	
}

TGen::Matrix3x3::operator std::string() const {
	std::stringstream ss;
	
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			ss << "[" << elements[x][y] << "]";
		}
		
		ss << "\n";
	}
	
	return ss.str();
}

