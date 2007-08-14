/*
 *  face.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

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
		Face(TGen::Geometry * geometry, const std::string & materialName);
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
