/*
 *  fpscamera.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

//// summary ///////////////////////////////////////////////////////////////////////////////////////
// This is a "first person shooter"-camera, it has the properties common to such cameras, like
// moving and rotating with constraints.
// It's a Camera, and thus a SceneNode and can be inserted into the scene graph at any point.
// Movement is applied in the move method which takes a vector in local direction, ie, 
// (1, 0, 0) will strafe the camera to the right, (0, 0, -1) will move it backwards.
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TGEN_RENDERER_FPSCAMERA_H
#define _TGEN_RENDERER_FPSCAMERA_H

#include "camera.h"
#include <tgen_math.h>

namespace TGen {
	class FPSCamera : public TGen::Camera {
	public:
		FPSCamera(const std::string & name, const TGen::Vector3 & position, const TGen::Rotation & orientation);
		~FPSCamera();
		
		void update(scalar dt);
		void move(const TGen::Vector3 & direction);
		void rotate(const TGen::Vector2 & rot);
		
	private:
		scalar orientationX, orientationY;
		TGen::Matrix4x4 moveMatrix;
	};
} // !TGen

#endif // !_TGEN_RENDERER_FPSCAMERA_H
