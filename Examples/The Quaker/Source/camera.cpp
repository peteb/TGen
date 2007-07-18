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
	, fovY(90.0f)
	, aspectRatio(1.0f)
	, near(0.1f)
	, far(100.0f)
	, lodNear(0.1f)
	, lodFar(100.0f)
	, projectionMatrix(TGen::Matrix4x4::Identity)
	, projUpdated(true)
{
		
}

/*
 fovY, aspectRatio, near, far, lodNear, lodFar
 */

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
}

