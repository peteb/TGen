/*
 *  fpscamera.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "fpscamera.h"

TGen::FPSCamera::FPSCamera(const std::string & name, const TGen::Vector3 & position, const TGen::Quaternion4 & orientation)
	: TGen::Camera(name, position, orientation)
	, orientationX(0.0f)
	, orientationY(0.0f)
{		
}

TGen::FPSCamera::~FPSCamera() {
	
}

void TGen::FPSCamera::update() {
	TGen::Clamp(orientationY, -TGen::HALF_PI, TGen::HALF_PI);
	
	if ((parent && parent->hasChanged()) || this->changed) {
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

		look.normalize();
		up = TGen::Vector3::CrossProduct(look, right).normalize();
		right = TGen::Vector3::CrossProduct(up, look).normalize();
		
		std::cout << "look: " << std::string(look) << std::endl;
		
		moveMatrix = TGen::Matrix4x4(-right, -up, look);
		
		if (parent)
			transform = parent->getTransform()/*.getInverse()*/ * TGen::Matrix4x4(right, up, look).getInverse() * TGen::Matrix4x4::Translation(position);
		else
			transform = TGen::Matrix4x4(right, up, look).getInverse() * TGen::Matrix4x4::Translation(position);
		
		this->changed = true;
	}
	
	updateChildren();
	
	this->changed = false;
}

void TGen::FPSCamera::move(const TGen::Vector3 & direction) {
	position += moveMatrix * direction;
	this->changed = true;
}

void TGen::FPSCamera::rotate(const TGen::Vector2 & rot) {
	orientationX += rot.x;
	orientationY += rot.y;
	this->changed = true;
}

