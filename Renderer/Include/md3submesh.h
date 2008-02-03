/*
 *  md3submesh.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/26/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MD3SUBMESH_H
#define _TGEN_RENDERER_MD3SUBMESH_H

#include "geometry.h"
#include "mesh_new.h"

namespace TGen {
	class VertexBuffer;
	class IndexBuffer;
	class Renderer;
	
	namespace MD3 {
		class Submesh : public TGen::NewMesh {
		public:
			Submesh(const std::string & materialName);
			~Submesh();
			
			void preRender(TGen::Renderer & renderer) const;
			void render(TGen::Renderer & renderer) const;

			TGen::MD3::Submesh * instantiate() const;
			
			
			TGen::VertexData * vb;
			TGen::VertexData * ib;
			TGen::PrimitiveType primitive;
			uint startIndex, indexCount;
		};
		
		
		
	} // !MD3
} // !TGen

#endif // !_TGEN_RENDERER_MD3SUBMESH_H

