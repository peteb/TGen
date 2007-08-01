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
	
	SceneNode::Update(parent, parentUpdated);
	
	//orientationY = 1.0;
	//TGen::Vector3 right = TGen::Vector3::CrossProduct(getOrientation(), getUp());
	//TGen::Vector3 newUp = TGen::Vector3::CrossProduct(getUp(), right);
	
	//right.Normalize();
	
	//TGen::Quaternion4 rot;
	
	//rot = TGen::Quaternion4::Rotation(TGen::Vector3(0.0f, 1.0f, 0.0f), TGen::Radian(orientationX)).Normalize();
	//rot *= TGen::Quaternion4::Rotation(TGen::Vector3(1.0f, 0.0f, 0.0f), TGen::Radian(orientationY)).Normalize();
	if (orientationY > TGen::HALF_PI)
		orientationY = TGen::HALF_PI;
	if (orientationY < -TGen::HALF_PI)
		orientationY = -TGen::HALF_PI;
	
	TGen::Vector3 up(0.0f, 1.0f, 0.0f), look(0.0f, 0.0f, 1.0f), right(1.0f, 0.0f, 0.0f);
	
	
	TGen::Matrix4x4 matY = TGen::Matrix4x4::Rotation(right, TGen::Radian(orientationY - TGen::PI));
	
	
	look = matY * look;
	//std::cout << "*** " << std::string(look) << std::endl;

	up = matY * up;
	up.Normalize();
	
	TGen::Matrix4x4 matX = TGen::Matrix4x4::Rotation(up, TGen::Radian(orientationX));
	//TGen::Matrix4x4 matY = TGen::Matrix4x4::Rotation(TGen::Vector3(1.0f, 0.0f, 0.0f), TGen::Radian(orientationY));
	scalar savedY = look.y;
	look = matX * look;
	right = matX * right;
	//matX *= TGen::Matrix4x4::Rotation(-right, TGen::Radian(orientationY));
	
	//TGen::Vector3 orientation = look.Normalize();
	TGen::Matrix4x4 mat(right.getNormalized(), up.getNormalized(), look.getNormalized());
	
	//std::cout << std::string(TGen::Vector3(rot)) << std::endl;
	//setOrientation(look);
	this->forward = look;
	this->forward.y = savedY;
	//std::cout << "--> " << std::string(look) << std::endl;
	
	// rot * TGen::Quaternion4(0.0f, 0.0f, 1.0f) * -rot
	transform = mat * TGen::Matrix4x4::Translation(getPosition());
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


