/*
 *  camera.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "camera.h"

TGen::Camera::Camera(const std::string & name, const TGen::Vector3 & position, const TGen::Quaternion4 & orientation) 
	: TGen::SceneNode(name, position, orientation)
	, fov(80.0f)
	, aspectRatio(1.0f)	
	, near(0.1f)
	, far(100.0f)
	, lodNear(0.1f)
	, lodFar(100.0f)
	, projectionMatrix(TGen::Matrix4x4::Identity)
	, projectionNeedUpdate(true)
{
	
}

TGen::Camera::~Camera() {
	
}

