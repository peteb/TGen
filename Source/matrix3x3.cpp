/*
 *  Matrix3x33x3.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/6/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "Matrix3x3.h"
#include "Vector2.h"
#include <cstring>
#include <sstream>

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

TGen::Matrix3x3::Matrix3x3(const TGen::Matrix3x3 & Matrix3x3) {
	memcpy(&elements, &Matrix3x3.elements, 3 * 3 * sizeof(float));
}

TGen::Matrix3x3::Matrix3x3(float e11, float e12, float e13, float e21, float e22, float e23, float e31, float e32, float e33) {
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
	memset(&elements, 0, 3 * 3 * sizeof(float));
}

TGen::Matrix3x3 & TGen::Matrix3x3::operator = (const TGen::Matrix3x3 & matrix) {
	memcpy(elements, matrix.elements, sizeof(float) * 3 * 3);
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

void TGen::Matrix3x3::get4x4(float * elements) const {
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

TGen::Matrix3x3 TGen::Matrix3x3::Translation(const TGen::Vector2 & Vector2) {
	TGen::Matrix3x3 Matrix3x3 = TGen::Matrix3x3::Identity;
	Matrix3x3.setOrigin(Vector2);
	return Matrix3x3;
}

TGen::Matrix3x3 TGen::Matrix3x3::Scaling(const TGen::Vector2 & scale) {
	TGen::Matrix3x3 Matrix3x3 = TGen::Matrix3x3::Identity;
	Matrix3x3.setScale(scale);
	return Matrix3x3;	
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

