/*
 *  md3mesh.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/26/07.
 *  Renamed by Peter Backman on 2/3/08.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MD3MESH_H
#define _TGEN_RENDERER_MD3MESH_H

#include "geometry.h"
#include "mesh_new.h"

namespace TGen {
	class VertexBuffer;
	class IndexBuffer;
	class Renderer;
	
	namespace MD3 {
		class Mesh : public TGen::NewMesh {
		public:
			Mesh(const std::string & materialName);
			~Mesh();
			
			void preRender(TGen::Renderer & renderer) const;
			void render(TGen::Renderer & renderer) const;

			TGen::MD3::Mesh * instantiate() const;
			
			
			TGen::VertexData * vb;
			TGen::VertexData * ib;
			TGen::PrimitiveType primitive;
			uint startIndex, indexCount;
		};
		
		
		
	} // !MD3
} // !TGen

#endif // !_TGEN_RENDERER_MD3MESH_H

