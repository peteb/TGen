/*
 *  face.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

//// summary ///////////////////////////////////////////////////////////////////////////////////////
// The face is the "atom" of the renderer, it has everything needed to render something.
// It has a Material, a SceneNode and a Geom. The material provides the properties for the rendering,
// like color and gpu programs, the scene node provides position and orientation, and the geom contains
// the shape of the face. All these can be shared between faces, but a face can only be attached
// to one scene node at a given time.
//
// keywords: synergy, facade
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TGEN_RENDERER_FACE_H
#define _TGEN_RENDERER_FACE_H

#include <tgen_math.h>
#include <string>

namespace TGen {
	class Material;
	class MaterialSource;
	class Geometry;
	class SceneNode;
	
	class Face {
	public:
		Face(TGen::Geometry * geometry, const std::string & materialName, TGen::SceneNode * node = NULL);
		~Face();
		
		void linkMaterial(TGen::MaterialSource & source);
		void unlinkMaterial();
				
		TGen::Geometry * getGeometry() const;
		TGen::Material * getMaterial() const;
		TGen::SceneNode * getSceneNode() const;
		std::string getMaterialName() const;
		TGen::Vector3 getWorldOrigin() const;
		
		friend class TGen::SceneNode;
		
	private:
		void attached(TGen::SceneNode * node);
		
		std::string materialName;
		TGen::Material * material;
		TGen::Geometry * geometry;
		TGen::SceneNode * sceneNode;
	}; 
	
	
} // !TGen

#endif // !_TGEN_RENDERER_FACE_H
