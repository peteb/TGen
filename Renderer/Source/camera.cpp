/*
 *  camera.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "camera.h"

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
#include <OpenGL/glu.h>

void TGen::Camera::update() {
	if (projectionChanged) {
		projectionMatrix = TGen::Matrix4x4::PerspectiveProjection(fov, aspectRatio, clipNear, clipFar);
		projectionChanged = false;
	}
		
	if ((parent && parent->hasChanged()) || this->changed) {		
		TGen::Quaternion4 front(0.0f, 0.0f, 1.0f);
		//TGen::Vector3 result = orientation * front * -orientation;
		
		//TGen::Vector3 result = TGen::Vector3(orientation);   // FULHACK
		
		/*if (parent)
			this->transform = parent->getTransform() * (TGen::Matrix4x4::Translation(position) * TGen::Matrix4x4::LookInDirection(result, up)).invert();
		else
			this->transform = (TGen::Matrix4x4::Translation(position) * TGen::Matrix4x4::LookInDirection(result, up)).invert();
		*/
		

		//this->transform = (TGen::Matrix4x4::Translation(position) * TGen::Matrix4x4::LookInDirection(result, up)).invert();
		
		/*if (parent)
			this->transform = parent->getTransform() * TGen::Matrix4x4::LookInDirection(result, up) * TGen::Matrix4x4::Translation(-TGen::Vector3(position.x, position.y, position.z));	// SKUM
		else
			this->transform = TGen::Matrix4x4::LookInDirection(result, up) * TGen::Matrix4x4::Translation(TGen::Vector3(position.x, -position.y, -position.z)); // FULHACK
			*/
		
		//std::cout << "POS: " << std::string(position) << " ORIENT: " << std::string(TGen::Vector3(orientation)) << " UP: " << std::string(up) << std::endl;
		
		//TGen::Vector3 fixedRight = TGen::Vector3::CrossProduct(up, result).getNormalized();
		//TGen::Vector3 fixedUp = TGen::Vector3::CrossProduct(result, fixedRight).getNormalized();
		//TGen::Vector3 fixedView = result.getNormalized();
		
		/*std::cout << "   fixed right: " << std::string(fixedRight) << std::endl;
		std::cout << "   fixed up: " << std::string(fixedUp) << std::endl;
		std::cout << "   fixed view: " << std::string(fixedView) << std::endl;
		
		TGen::Vector3 f = TGen::Vector3(orientation);
		TGen::Vector3 s = TGen::Vector3::CrossProduct(f, up).getNormalized();
		TGen::Vector3 u = TGen::Vector3::CrossProduct(s, f).getNormalized();
		
		*/
		
		TGen::Matrix4x4 fixedMat = orientation; //(fixedRight, fixedUp, -fixedView);

		
		//TGen::Matrix4x4 mat(s, u, -f);
		
		//std::cout << "---" << std::endl << std::string(parent->getTransform() * fixedMat  * TGen::Matrix4x4::Translation(-position)) << std::endl;
		
		if (parent)
			this->transform = parent->getTransform() * fixedMat  * TGen::Matrix4x4::Translation(-position);
		else
			this->transform = fixedMat  * TGen::Matrix4x4::Translation(-position);

		
		/*std::cout << "TRANSFORM:" << std::endl << std::string(this->transform) << std::endl << std::endl;
		std::cout << "LOOKDIR: " << std::endl << std::string(TGen::Matrix4x4::LookInDirection(fixedView, up) * TGen::Matrix4x4::Translation(-position)) << std::endl;
		
		TGen::Vector3 lookat = position + TGen::Vector3(orientation);
	
		GLfloat glMat[16];
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(position.x, position.y, position.z, lookat.x, lookat.y, lookat.z, up.x, up.y, up.z);
			
//		glMultMatrixf(testMat);
	//	glTranslatef(-position.x, -position.y, -position.z);
		glGetFloatv(GL_MODELVIEW_MATRIX, glMat);
		
		std::cout << "GLTRANS: " << std::endl;
		for (int y = 0; y < 4; ++y) {
			for (int x = 0; x < 4; ++x) {
				std::cout << "[" << glMat[x * 4 + y] << "]";
			}
			
			std::cout << std::endl;
		}
		std::cout << std::endl;
		*/
		
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
