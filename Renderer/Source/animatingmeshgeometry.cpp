/*
 *  animatingmeshgeometry.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/14/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "animatingmeshgeometry.h"

TGen::AnimatingMeshGeometry::AnimatingMeshGeometry(const std::string & mesh, scalar startAt)
	: TGen::MeshGeometry(mesh)
	, startedAt(startAt)
	, lastUpdate(0.0f)
	, updateInterval(0.1f)
{
}

TGen::AnimatingMeshGeometry::~AnimatingMeshGeometry() {
	
}

void TGen::AnimatingMeshGeometry::preRender(TGen::Renderer & renderer) const {
	
}

void TGen::AnimatingMeshGeometry::render(TGen::Renderer & renderer) const {
	
}

void TGen::AnimatingMeshGeometry::update(const TGen::Camera & camera, scalar distance, scalar time) {
	if (time - lastUpdate >= updateInterval) {
		/*
		 mesh->getFrame(time - startedAt, this->meshData); // meshData har vb och ib och lite info, numindices, primitive, numvertices, allt som behövs för DrawIndexedPrimitive
		 MeshData
		 
		 */
		lastUpdate = time;
	}
	
}
