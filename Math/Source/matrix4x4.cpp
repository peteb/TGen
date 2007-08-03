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
#include "angle.h"
#include <cmath>
#include <sstream>

TGen::Matrix4x4 TGen::Matrix4x4::Identity(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

TGen::Matrix4x4::Matrix4x4() {
	Clear();
}

TGen::Matrix4x4::Matrix4x4(const TGen::Vector3 & x, const TGen::Vector3 & y, const TGen::Vector3 & z) {
	Clear();
	elements[0][0] = x.x;
	elements[0][1] = x.y;
	elements[0][2] = x.z;
	elements[1][0] = y.x;
	elements[1][1] = y.y;
	elements[1][2] = y.z;
	elements[2][0] = z.x;
	elements[2][1] = z.y;
	elements[2][2] = z.z;
	elements[3][3] = 1.0f;
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

TGen::Vector3 TGen::Matrix4x4::getX() const {
	return TGen::Vector3(elements[0][0], elements[0][1], elements[0][2]);
}

TGen::Vector3 TGen::Matrix4x4::getY() const {
	return TGen::Vector3(elements[1][0], elements[1][1], elements[1][2]);
}

TGen::Vector3 TGen::Matrix4x4::getZ() const {
	return TGen::Vector3(elements[2][0], elements[2][1], elements[2][2]);	
}

void TGen::Matrix4x4::setX(const TGen::Vector3 & xaxis) {
	elements[0][0] = xaxis.x;
	elements[0][1] = xaxis.y;
	elements[0][2] = xaxis.z;	
}

void TGen::Matrix4x4::setY(const TGen::Vector3 & yaxis) {
	elements[1][0] = yaxis.x;
	elements[1][1] = yaxis.y;
	elements[1][2] = yaxis.z;
}

void TGen::Matrix4x4::setZ(const TGen::Vector3 & zaxis) {
	elements[2][0] = zaxis.x;
	elements[2][1] = zaxis.y;
	elements[2][2] = zaxis.z;
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
	
	return ret;
}

TGen::Matrix4x4 & TGen::Matrix4x4::operator *= (const TGen::Matrix4x4 & matrix) {
	*this = *this * matrix;
	
	return *this;
}

TGen::Matrix4x4 TGen::Matrix4x4::operator * (const TGen::Matrix4x4 & matrix) const {
	Matrix4x4 matRet;
	
	for (int r = 0; r < 4; r++) { 
		for (int c = 0; c < 4; c++) { 
			for (int i = 0; i < 4; i++) { 
				matRet.elements[r][c] += matrix.elements[r][i] * elements[i][c]; 
			} 
		} 
	} 
		
	return matRet;
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

TGen::Vector3 TGen::Matrix4x4::getOrigin() const {
	return TGen::Vector3(elements[3][0], elements[3][1], elements[3][2]);
}

void TGen::Matrix4x4::setScale(const TGen::Vector3 & scale) {
	elements[0][0] = scale.x;
	elements[1][1] = scale.y;
	elements[2][2] = scale.z;
}

void TGen::Matrix4x4::get4x4(scalar * elements) const {
	//std::cout << "HEJ" << std::endl;
	memcpy(elements, this->elements, sizeof(scalar) * 4 * 4);
}

TGen::Matrix4x4 & TGen::Matrix4x4::Transpose() {
	
	
	return *this;
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
	
	ret.elements[0][0] = scalar(2.0 / (right - left));
	ret.elements[1][1] = 2.0 / (top - bottom);
	ret.elements[2][2] = scalar(0.0); //(T)-2.0 / (far - near);
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

TGen::Matrix4x4 TGen::Matrix4x4::PerspectiveProjection(const TGen::Angle & fieldOfViewY, scalar fieldOfViewAspect, scalar near, scalar far) {
	Matrix4x4 ret;
	float f = 1.0f / tanf(TGen::Radian(fieldOfViewY).angle / 2.0f);
				
	ret.elements[0][0] = f / fieldOfViewAspect;
	ret.elements[1][1] = f;
	ret.elements[2][2] = (far + near) / (near - far);
	ret.elements[3][2] = (2 * far * near) / (near - far);
	ret.elements[2][3] = -1.0;
				
	return ret;
}

TGen::Matrix4x4 TGen::Matrix4x4::Rotation(const TGen::Vector3 & axis, const TGen::Angle & angle) {
	Matrix4x4 ret;
	scalar c = cos(TGen::Radian(angle).angle);
	scalar s = sin(TGen::Radian(angle).angle);
	scalar omc = 1.0 - c;
	
	ret.elements[0][0] = 1.0 + omc * (axis.x * axis.x - 1.0);
	ret.elements[1][0] = -axis.z * s + omc * axis.x * axis.y;
	ret.elements[2][0] = axis.y * s + omc * axis.x * axis.z;
	ret.elements[0][1] = axis.z * s + omc * axis.x * axis.y;
	ret.elements[1][1] = 1.0 + omc * (axis.y * axis.y - 1.0);
	ret.elements[2][1] = -axis.x * s + omc * axis.y * axis.z;
	ret.elements[0][2] = -axis.y * s + omc * axis.x * axis.z;
	ret.elements[1][2] = axis.x * s + omc * axis.y * axis.z;
	ret.elements[2][2] = 1.0 + omc * (axis.z * axis.z - 1.0);
	ret.elements[3][3] = 1.0;
	return ret;
}

TGen::Matrix4x4 TGen::Matrix4x4::RotationX(const TGen::Angle & angle) {
	Matrix4x4 ret;
	ret.elements[0][0] = 1.0;
	ret.elements[1][1] = TGen::Cos(angle);
	ret.elements[2][1] = -TGen::Sin(angle);
	ret.elements[1][2] = TGen::Sin(angle);
	ret.elements[2][2] = TGen::Cos(angle);
	ret.elements[3][3] = 1.0;
	
	return ret;
}

TGen::Matrix4x4 TGen::Matrix4x4::RotationY(const TGen::Angle & angle) {
	Matrix4x4 ret;
	ret.elements[0][0] = TGen::Cos(angle);
	ret.elements[2][0] = TGen::Sin(angle);
	ret.elements[1][1] = 1.0;
	ret.elements[0][2] = -TGen::Sin(angle);
	ret.elements[2][2] = TGen::Cos(angle);
	ret.elements[3][3] = 1.0;
	
	return ret;
}

TGen::Matrix4x4 TGen::Matrix4x4::RotationZ(const TGen::Angle & angle) {
	Matrix4x4 ret;
	ret.elements[0][0] = TGen::Cos(angle);
	ret.elements[1][0] = -TGen::Sin(angle);
	ret.elements[0][1] = TGen::Sin(angle);
	ret.elements[1][1] = TGen::Cos(angle);
	ret.elements[2][2] = 1.0;	
	ret.elements[3][3] = 1.0;
	
	return ret;
}

TGen::Matrix4x4 TGen::Matrix4x4::LookAt(const TGen::Vector3 & position, const TGen::Vector3 & eye, const TGen::Vector3 & up) {
	Matrix4x4 ret = Matrix4x4::Identity;
	Vector3 forward = (eye - position).Normalize();
	Vector3 UP = up.getNormalized();
	Vector3 right = Vector3::CrossProduct(forward, UP).Normalize();
	Vector3 newUp = Vector3::CrossProduct(forward, right).Normalize();
	
	ret = Matrix4x4(right, -newUp, -forward);
	//ret = ret.Transpose();
	ret *= Matrix4x4::Translation(-position);
	
	return ret;				
}

TGen::Matrix4x4 TGen::Matrix4x4::LookInDirection(const TGen::Vector3 & direction, const TGen::Vector3 & up) {	
	TGen::Vector3 look, right, newUp;
	look = direction.getNormalized();
	right = TGen::Vector3::CrossProduct(up, look);
	newUp = TGen::Vector3::CrossProduct(look, right);
	
	return Matrix4x4(right, newUp, look);
}

void TGen::Matrix4x4::Clear() {
	memset(&elements, 0, 4 * 4 * sizeof(scalar));
}

TGen::Matrix4x4 & TGen::Matrix4x4::Inverse() {
	TGen::Matrix4x4 mat;
	
	scalar wg;
	scalar b1, b2, b3, b4, b5, b6;
	
	b1 = elements[2][2] * elements[3][3] - elements[2][3] *elements[3][2]; b2 = elements[2][0] * elements[3][3] - elements[2][3] * elements[3][0];
	b3 = elements[2][0] * elements[3][1] - elements[2][1] * elements[3][0]; b4 = elements[2][2] * elements[3][1] - elements[2][1] * elements[3][2];
	b5 = elements[2][3] * elements[3][1] - elements[2][1] * elements[3][3]; b6 = elements[2][2] * elements[3][0] - elements[2][0] * elements[3][2];
	
	mat.elements[0][0] = elements[1][1] * b1+elements[1][2] * b5 - elements[1][3] * b4;
	mat.elements[1][0] = elements[1][2] * b2+elements[1][3] * b6 - elements[1][0] * b1;
	mat.elements[2][0] = elements[1][3] * b3-elements[1][0] * b5 - elements[1][1] * b2;
	mat.elements[3][0] = elements[1][0] * b4-elements[1][1] * b6 - elements[1][2] * b3;
	
	wg = mat.elements[0][0] * elements[0][0] + mat.elements[1][0] * elements[0][1] + mat.elements[2][0] * elements[0][2] + mat.elements[3][0] * elements[0][3];
	if (wg == 0)
		return *this;
	
	wg = 1.0f / wg;
	mat.elements[0][0] *= wg; mat.elements[1][0] *= wg; mat.elements[2][0] *= wg; mat.elements[3][0] *= wg;
	
	b1=elements[3][2]*elements[0][3]-elements[3][3]*elements[0][2]; b2=elements[3][0]*elements[0][3]-elements[3][3]*elements[0][0];
	b3=elements[3][0]*elements[0][1]-elements[3][1]*elements[0][0]; b4=elements[3][2]*elements[0][1]-elements[3][1]*elements[0][2];
	b5=elements[3][3]*elements[0][1]-elements[3][1]*elements[0][3]; b6=elements[3][2]*elements[0][0]-elements[3][0]*elements[0][2];
	
	mat.elements[0][1]=wg*(-elements[2][1]*b1-elements[2][2]*b5+elements[2][3]*b4);
	mat.elements[1][1]=wg*(-elements[2][2]*b2-elements[2][3]*b6+elements[2][0]*b1);
	mat.elements[2][1]=wg*(-elements[2][3]*b3+elements[2][0]*b5+elements[2][1]*b2);
	mat.elements[3][1]=wg*(-elements[2][0]*b4+elements[2][1]*b6+elements[2][2]*b3);
	
	b1=elements[0][2]*elements[1][3]-elements[0][3]*elements[1][2]; b2=elements[0][0]*elements[1][3]-elements[0][3]*elements[1][0];
	b3=elements[0][0]*elements[1][1]-elements[0][1]*elements[1][0]; b4=elements[0][2]*elements[1][1]-elements[0][1]*elements[1][2];
	b5=elements[0][3]*elements[1][1]-elements[0][1]*elements[1][3]; b6=elements[0][2]*elements[1][0]-elements[0][0]*elements[1][2];
	
	mat.elements[0][2]=wg*(elements[3][1]*b1+elements[3][2]*b5-elements[3][3]*b4);
	mat.elements[1][2]=wg*(elements[3][2]*b2+elements[3][3]*b6-elements[3][0]*b1);
	mat.elements[2][2]=wg*(elements[3][3]*b3-elements[3][0]*b5-elements[3][1]*b2);
	mat.elements[3][2]=wg*(elements[3][0]*b4-elements[3][1]*b6-elements[3][2]*b3);
	
	b1=elements[1][2]*elements[2][3]-elements[1][3]*elements[2][2]; b2=elements[1][0]*elements[2][3]-elements[1][3]*elements[2][0];
	b3=elements[1][0]*elements[2][1]-elements[1][1]*elements[2][0]; b4=elements[1][2]*elements[2][1]-elements[1][1]*elements[2][2];
	b5=elements[1][3]*elements[2][1]-elements[1][1]*elements[2][3]; b6=elements[1][2]*elements[2][0]-elements[1][0]*elements[2][2];
	
	mat.elements[0][3]=wg*(-elements[0][1]*b1-elements[0][2]*b5+elements[0][3]*b4);
	mat.elements[1][3]=wg*(-elements[0][2]*b2-elements[0][3]*b6+elements[0][0]*b1);
	mat.elements[2][3]=wg*(-elements[0][3]*b3+elements[0][0]*b5+elements[0][1]*b2);
	mat.elements[3][3]=wg*(-elements[0][0]*b4+elements[0][1]*b6+elements[0][2]*b3);		
	
	*this = mat;
	
	return *this;
}

TGen::Matrix4x4 TGen::Matrix4x4::getInverse() const {
	TGen::Matrix4x4 ret(*this);
	return ret.Inverse();
}

