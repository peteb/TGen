/*
 *  face_new.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/3/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_FACE_NEW_H
#define _TGEN_RENDERER_FACE_NEW_H

#include <tgen_math.h>

namespace TGen {
	class NewMeshInstance;
	class Material;
	class SceneNode;
	
	class NewFace {
	public:
		NewFace(TGen::NewMeshInstance * mesh, TGen::Material * material, const TGen::SceneNode * sceneNode);
		
		TGen::NewMeshInstance * getMesh() const;
		TGen::Material * getMaterial() const;
		TGen::SceneNode const * getSceneNode() const;

		TGen::Vector3 getWorldOrigin() const;
		
	protected:
		TGen::NewMeshInstance * mesh;
		TGen::Material * material;
		TGen::SceneNode const * sceneNode; 
	};
	
} // !TGen

#endif // !_TGEN_RENDERER_FACE_NEW_H
