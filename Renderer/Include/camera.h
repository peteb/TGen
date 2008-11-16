/*
 *  camera.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

//// summary ///////////////////////////////////////////////////////////////////////////////////////
// This is the base class for cameras, it provides the minimum information needed for a camera.
// It's a SceneNode and can be inserted anywhere in the tree.
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TGEN_RENDERER_CAMERA_H
#define _TGEN_RENDERER_CAMERA_H

#include "scenenode.h"
#include "lodinfo.h"

namespace TGen {

	class Camera : public TGen::SceneNode {
	public:
		Camera(const std::string & name, const TGen::Vector3 & position, const TGen::Rotation & rotation);
		virtual ~Camera();
		
		virtual void update(scalar dt);
		void calculateWorldBV();
		void setAspectRatio(scalar ratio);
		void setFov(const TGen::Degree & angle);
		void setClip(scalar near, scalar far);
		void setLod(scalar near, scalar far);

		const TGen::LodInfo & getLod() const;
		
		const TGen::Matrix4x4 & getProjection() const;

	protected:
		TGen::Degree fov;
		scalar aspectRatio;
		TGen::LodInfo lodInfo;
		bool projectionChanged;
		TGen::Matrix4x4 projectionMatrix;		
	};	
	
} // !TGen

#endif // !_TGEN_RENDERER_CAMERA_H
