/*
 *  animatingmeshgeometry.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/14/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_ANIMATINGMESHGEOMETRY_H
#define _TGEN_RENDERER_ANIMATINGMESHGEOMETRY_H

#include "geometry.h"
#include "meshgeometry.h"

namespace TGen {
	class Camera;
	
	class AnimatingMeshGeometry : public TGen::MeshGeometry {
	public:	
		AnimatingMeshGeometry(const std::string & mesh, scalar startAt);
		~AnimatingMeshGeometry();
		
		void preRender(TGen::Renderer & renderer) const;
		void render(TGen::Renderer & renderer) const;
		void update(const TGen::Camera & camera, scalar distance, scalar time);
		
	private:
		scalar startedAt, updateInterval, lastUpdate;
	};
	
} // !TGen

#endif // !_TGEN_RENDERER_ANIMATINGMESHGEOMETRY_H
