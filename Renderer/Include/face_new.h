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
	class ModelRenderProperties;
	
	class NewFace {
	public:
		NewFace(TGen::NewMeshInstance * mesh, TGen::Material * material, const TGen::SceneNode * sceneNode, const TGen::ModelRenderProperties * renderProperties);
		
		TGen::NewMeshInstance * getMesh() const;
		TGen::Material * getMaterial() const;
		TGen::SceneNode const * getSceneNode() const;
		const TGen::ModelRenderProperties * getRenderProperties() const;
		void setSceneNode(const TGen::SceneNode * node);
		void setMaterial(TGen::Material * material);
		
		TGen::Vector3 getWorldOrigin() const;
		
	protected:
		const TGen::ModelRenderProperties * renderProperties;
		TGen::NewMeshInstance * mesh;
		TGen::Material * material;
		const TGen::SceneNode * sceneNode; 
	};
	
} // !TGen

#endif // !_TGEN_RENDERER_FACE_NEW_H
