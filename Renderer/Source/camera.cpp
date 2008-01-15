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
		
	if ((parent && parent->hasChanged()) || this->changed) {		
		TGen::Quaternion4 front(0.0f, 0.0f, 1.0f);
		//TGen::Vector3 result = orientation * front * -orientation;
		
		TGen::Vector3 result = TGen::Vector3(orientation);   // FULHACK
		//result.z = -result.z;	// FULHACK
		
		
		//TGen::Vector3 fronten(0.0f, 0.0f, 1.0f);
		//std::cout << "fixed:::::::: " << std::string(TGen::Matrix4x4(resse) * fronten) << std::endl;
		
		//std::cout << "******************************** CAMERA ORIENTATION " << std::string(TGen::Vector3(result)) << std::endl;
		
		/*if (parent)
			this->transform = parent->getTransform() * (TGen::Matrix4x4::Translation(position) * TGen::Matrix4x4::LookInDirection(result, up)).invert();
		else
			this->transform = (TGen::Matrix4x4::Translation(position) * TGen::Matrix4x4::LookInDirection(result, up)).invert();
		*/
		

		//this->transform = (TGen::Matrix4x4::Translation(position) * TGen::Matrix4x4::LookInDirection(result, up)).invert();
		
		if (parent)
			this->transform = parent->getTransform() * TGen::Matrix4x4::LookInDirection(result, up) * TGen::Matrix4x4::Translation(-TGen::Vector3(position.x, position.y, position.z));
		else
			this->transform = TGen::Matrix4x4::LookInDirection(result, up) * TGen::Matrix4x4::Translation(TGen::Vector3(position.x, -position.y, -position.z)); // FULHACK
			
		
		//std::cout << "TRANS::::: " << std::endl << std::string(TGen::Matrix4x4::LookInDirection(result, up).invert()) << std::endl;
		this->changed = true;
	}
	
	updateChildren();
	
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
