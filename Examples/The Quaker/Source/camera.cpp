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
	, fovY(60.0f)
	, aspectRatio(1.0f)
	, near(10.0f)
	, far(800.0f)
	, lodNear(0.1f)
	, lodFar(100.0f)
	, projectionMatrix(TGen::Matrix4x4::Identity)
	, projUpdated(true)
{
		
}

const TGen::Matrix4x4 & Camera::getProjection() const {
	return projectionMatrix;
}

void Camera::Update(const TGen::Matrix4x4 & parent, bool parentUpdated) {
	if (projUpdated) {
		std::cout << "updating cam projection" << std::endl;
		projectionMatrix = TGen::Matrix4x4::PerspectiveProjection(fovY, aspectRatio, near, far);
		projUpdated = false;
	}
	
	SceneNode::Update(parent, parentUpdated);
	transform = TGen::Matrix4x4::LookInDirection(getOrientation(), getUp()) * TGen::Matrix4x4::Translation(getPosition());
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
