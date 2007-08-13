/*
 *  camera.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_CAMERA_H
#define _TGEN_RENDERER_CAMERA_H

#include "scenenode.h"

namespace TGen {
	class Camera : public TGen::SceneNode {
	public:
		Camera(const std::string & name, const TGen::Vector3 & position, const TGen::Quaternion4 & orientation = TGen::Quaternion4(0.0f, 0.0f, 1.0f));
		~Camera();
		
		
	protected:
		TGen::Degree fov;
		scalar aspectRatio, near, far, lodNear, lodFar;
		bool projectionNeedUpdate;
		TGen::Matrix4x4 projectionMatrix, moveMatrix;		
	};	
	
} // !TGen

#endif // !_TGEN_RENDERER_CAMERA_H
