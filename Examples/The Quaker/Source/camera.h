/*
 *  camera.h
 *  The Quaker
 *
 *  Created by Peter Backman on 7/18/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _THEQUAKER_CAMERA_H
#define _THEQUAKER_CAMERA_H

#include "scenenode.h"

class Camera : public SceneNode {
public:	
	Camera(const std::string & name);

	void Update(const TGen::Matrix4x4 & parent, bool parentUpdated);	
	
	const TGen::Matrix4x4 & getProjection() const;
	void CalculateBV();
	void setAspectRatio(float ratio);
	void setNear(scalar near);
	void setFar(scalar far);
	void Move(const TGen::Vector3 & direction);
	
	//TGen::Quaternion4 orientationX, orientationY;
	scalar orientationX, orientationY;
	
private:
	scalar fovY, aspectRatio, near, far, lodNear, lodFar;
	bool projUpdated;
	TGen::Matrix4x4 projectionMatrix, moveMatrix;
	
};

#endif // !_THEQUAKER_CAMERA_H
