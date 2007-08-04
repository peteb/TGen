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
	right = -right;
	up = -up;
	
	look.Normalize();
	up = TGen::Vector3::CrossProduct(look, right).Normalize();
	right = TGen::Vector3::CrossProduct(up, look).Normalize();
	
	moveMatrix = TGen::Matrix4x4(-right, -up, look);
	transform = TGen::Matrix4x4(right, up, look).getInverse() * TGen::Matrix4x4::Translation(getPosition());
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

void Camera::Move(const TGen::Vector3 & direction) {
	TGen::Vector3 fixedDir = moveMatrix * direction;
	
	setPosition(getPosition() + fixedDir);
}

