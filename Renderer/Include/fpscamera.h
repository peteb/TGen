/*
 *  fpscamera.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_FPSCAMERA_H
#define _TGEN_RENDERER_FPSCAMERA_H

#include "camera.h"
#include <tgen_math.h>

namespace TGen {
	class FPSCamera : public TGen::Camera {
	public:
		FPSCamera(const std::string & name, const TGen::Vector3 & position, const TGen::Quaternion4 & orientation = TGen::Quaternion4(0.0f, 0.0f, 1.0f));
		~FPSCamera();
		
		void update();
		void move(const TGen::Vector3 & direction);
		void rotate(const TGen::Vector2 & rot);
		
	private:
		TGen::Radian orientationX, orientationY;
		TGen::Matrix4x4 moveMatrix;
	};
} // !TGen

#endif // !_TGEN_RENDERER_FPSCAMERA_H
