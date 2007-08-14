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
	, clipNear(0.1f)
	, clipFar(100.0f)
	, lodNear(0.1f)
	, lodFar(100.0f)
	, projectionMatrix(TGen::Matrix4x4::Identity)
	, projectionChanged(true)
{
	
}

TGen::Camera::~Camera() {
	
}

void TGen::Camera::update() {
	if (projectionChanged) {
		projectionMatrix = TGen::Matrix4x4::PerspectiveProjection(fov, aspectRatio, clipNear, clipFar);
		projectionChanged = false;
	}
	
	TGen::SceneNode::update();
}

void TGen::Camera::calculateWorldBV() {
	
}

void TGen::Camera::setAspectRatio(scalar ratio) {
	aspectRatio = ratio;
	projectionChanged = true;
}

void TGen::Camera::setFov(const TGen::Degree & angle) {
	fov = angle;
	projectionChanged = true;
}

void TGen::Camera::setClip(scalar near, scalar far) {
	clipNear = near;
	clipFar = far;
	projectionChanged = true;
}

void TGen::Camera::setLod(scalar near, scalar far) {
	lodNear = near;
	lodFar = far;
	projectionChanged = true;
}

scalar TGen::Camera::getLodNear() const {
	return lodNear;
}

scalar TGen::Camera::getLodFar() const {
	return lodFar;
}

scalar TGen::Camera::getClipNear() const {
	return clipNear;
}

scalar TGen::Camera::getClipFar() const {
	return clipFar;
}

const TGen::Matrix4x4 & TGen::Camera::getProjection() const {
	return projectionMatrix;
}
