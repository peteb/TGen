/*
 *  camera.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "camera.h"

using TGen::scalar;

TGen::Camera::Camera(const std::string & name, const TGen::Vector3 & position, const TGen::Rotation & orientation) 
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

void TGen::Camera::update(scalar dt) {
	if (projectionChanged) {
		projectionMatrix = TGen::Matrix4x4::PerspectiveProjection(fov, aspectRatio, clipNear, clipFar);
		projectionChanged = false;
	}
		
	if ((parent && parent->hasChanged()) || this->changed) {				
		TGen::Matrix4x4 fixedMat = orientation; //(fixedRight, fixedUp, -fixedView);

		if (parent)
			this->transform = fixedMat  * parent->getTransform() * TGen::Matrix4x4::Translation(position);
		else
			this->transform = fixedMat  * TGen::Matrix4x4::Translation(position);

		this->transform.invert();
		this->changed = true;
	}
	
	updateChildren(dt);
	
	this->changedSinceLastCheck = this->changed;
	this->changed = false;
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
