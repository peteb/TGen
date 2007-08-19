/*
 *  meshgeometry.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/14/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

//// summary ///////////////////////////////////////////////////////////////////////////////////////
// Geometry that is linked against a mesh source, meshes can be shared between geometries.
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TGEN_RENDERER_MESHGEOMETRY_H
#define _TGEN_RENDERER_MESHGEOMETRY_H

#include "geometry.h"

namespace TGen {
	class MeshSource;
	
	class MeshGeometry : public TGen::Geometry {
	public:	
		MeshGeometry(const std::string & mesh);
		virtual ~MeshGeometry();

		virtual void linkMesh(TGen::MeshSource & source);
		virtual void unlinkMesh();
		
		void * getMesh() const;
		std::string getMeshName() const;
	
		virtual void preRender(TGen::Renderer & renderer) const;
		virtual void render(TGen::Renderer & renderer) const;
		virtual void update(const TGen::Camera & camera, scalar distance, scalar time);
		
		virtual TGen::Vector3 getMax() const;
		virtual TGen::Vector3 getMin() const;
		virtual TGen::Vector3 getOrigin() const;
		
	protected:
		std::string meshName;
		void * mesh;
	};
	
	
} // !TGen

#endif // !_TGEN_RENDERER_MESHGEOMETRY_H
