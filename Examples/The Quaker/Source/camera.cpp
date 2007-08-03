/*
 *  camera.cpp
 *  The Quaker
 *
 *  Created by Peter Backman on 7/18/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "camera.h"

Camera::Camera(const std::string & name) 
	: SceneNode(name)
	, fovY(80.0f)
	, aspectRatio(1.0f)
	, near(10.0f)
	, far(1300.0f)
	, lodNear(0.1f)
	, lodFar(100.0f)
	, projectionMatrix(TGen::Matrix4x4::Identity)
	, projUpdated(true)
	, orientationX(0.0f)
	, orientationY(0.0f)
{
}

const TGen::Matrix4x4 & Camera::getProjection() const {
	return projectionMatrix;
}

void Camera::Update(const TGen::Matrix4x4 & parent, bool parentUpdated) {
	if (projUpdated) {
		//std::cout << "updating cam projection" << std::endl;
		projectionMatrix = TGen::Matrix4x4::PerspectiveProjection(TGen::Degree(fovY), aspectRatio, near, far);
		projUpdated = false;
	}
	
	//SceneNode::Update(parent, parentUpdated);
	
	//orientationY = 1.0;
	//TGen::Vector3 right = TGen::Vector3::CrossProduct(getOrientation(), getUp());
	//TGen::Vector3 newUp = TGen::Vector3::CrossProduct(getUp(), right);
	
	//right.Normalize();
	
	//TGen::Quaternion4 rot;
	//orientationY = 0.0;
	//orientationX = 0.0;
	if (orientationY > TGen::HALF_PI)
		orientationY = TGen::HALF_PI;
	if (orientationY < -TGen::HALF_PI)
		orientationY = -TGen::HALF_PI;
	
	
	TGen::Vector3 up(0.0f, 1.0f, 0.0f), look(0.0f, 0.0f, 1.0f), right(1.0f, 0.0f, 0.0f);
	
	TGen::Matrix4x4 rot;
	rot = TGen::Matrix4x4::Rotation(right, TGen::Radian(orientationY));
	look = rot * look;
	up = rot * up;

	
	rot = TGen::Matrix4x4::RotationY(TGen::Radian(orientationX));
	look = rot * look;
	right = rot * right;
	//up = rot * up;
	
		
	right = -right;
	up = -up;
	
	//std::cout << "right: " << std::string(right) << " up: " << std::string(up) << " look: " << std::string(look) << std::endl;

	
	look.Normalize();
	up = TGen::Vector3::CrossProduct(look, right).Normalize();
	right = TGen::Vector3::CrossProduct(up, look).Normalize();
	
	transform = TGen::Matrix4x4(right, up, look).Inverse() * TGen::Matrix4x4::Translation(getPosition());
	this->forward = look;
	this->right = right;
	
	/*TGen::Matrix4x4 matY = TGen::Matrix4x4::Rotation(right, TGen::Radian(orientationY));
	look = matY * look;
	up = matY * up;
	
	std::cout << "after X look: " << std::string(look) << " up: " << std::string(up) << " right: " << std::string(right) << std::endl;
	
	TGen::Matrix4x4 matX = TGen::Matrix4x4::Rotation(up, TGen::Radian(orientationX));
	look = matX * look;
	right = matX * right;
	up = matX * up;
	//up = -TGen::Vector3::CrossProduct(right, look);
	
	
	std::cout << "after Y look: " << std::string(look) << " up: " << std::string(up) << " right: " << std::string(right) << std::endl;

	TGen::Matrix4x4 mat(-right.getNormalized(), -up.getNormalized(), look.getNormalized());
	
	//std::cout << std::string(mat) << std::endl;
	
	transform = parent * mat * TGen::Matrix4x4::Translation(getPosition());
	this->forward = look;
	*/
	//rot = TGen::Quaternion4::Rotation(TGen::Vector3(0.0f, 1.0f, 0.0f), TGen::Radian(orientationX)).Normalize();
	//rot *= TGen::Quaternion4::Rotation(TGen::Vector3(1.0f, 0.0f, 0.0f), TGen::Radian(orientationY)).Normalize();

	
	/*TGen::Vector3 up(0.0f, 1.0f, 0.0f), look(0.0f, 0.0f, 1.0f), right(1.0f, 0.0f, 0.0f);
	
	
	TGen::Matrix4x4 matY = TGen::Matrix4x4::Rotation(right, TGen::Radian(orientationY - TGen::PI));
	look = matY * look;
	up = matY * up;
	scalar savedY = look.y;
	
	// look, up är här inverterade, z fram (1 egentligen) är -1, z bak är 1, y upp (1 egentligen) är -1, y ner är 1
	
	TGen::Matrix4x4 matX = TGen::Matrix4x4::Rotation(TGen::Vector3(0.0f, 1.0f, 0.0f), TGen::Radian(orientationX));
	look = matX * look;
	right = matX * right;
	up = matX * up;
	
	std::cout << "look: " << std::string(look.getNormalized()) << " saved Y: " << savedY <<  " right: " << std::string(right) << " up: " << std::string(up) << std::endl;
	// look är korrekt, right och up är inverterade
	
	// SKRIV OM ALLT MED EXAKTA UTRÄKNINGAR
	//matX *= TGen::Matrix4x4::Rotation(-right, TGen::Radian(orientationY));
	
	//TGen::Vector3 orientation = look.Normalize();
	TGen::Matrix4x4 mat(right.getNormalized(), up.getNormalized(), look.getNormalized());
	
	//std::cout << std::string(TGen::Vector3(rot)) << std::endl;
	//setOrientation(look);
	//this->forward = look;
	//	this->forward = (mat)* TGen::Vector3(0.0f, 0.0f, 1.0f);
	
	//std::cout << "--> " << std::string(look) << std::endl;
	
	// rot * TGen::Quaternion4(0.0f, 0.0f, 1.0f) * -rot
	//std::cout << "forward: " << std::string(mat2.getZ()) << " look: " << std::string(look) << std::endl;
	this->forward = look;
	
	transform = parent * mat * TGen::Matrix4x4::Translation(getPosition());
	//this->forward = transform.getZ().getNormalized();
*/
}

void Camera::CalculateBV() {
	TGen::Vector3 min(-0.1f, -0.1f, -0.1f), max(0.1f, 0.1f, 0.1f);
	
	objectBoundingSphere.radius = std::max(min.getMagnitude(), max.getMagnitude());
	objectBoundingBox.Calculate(min, max);	
	
	//CalculateWorldBV();
}

void Camera::setAspectRatio(float ratio) {
	aspectRatio = ratio;
	projUpdated = true;
}

void Camera::setNear(scalar near) {
	this->near = near;
	projUpdated = true;
}

void Camera::setFar(scalar far) {
	this->far = far;
	projUpdated = true;
}


