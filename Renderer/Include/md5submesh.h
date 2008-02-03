/*
 *  md5submesh.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 1/23/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MD5SUBMESH_H
#define _TGEN_RENDERER_MD5SUBMESH_H

#include <tgen_renderer.h>

namespace TGen {
	namespace MD5 {
		class Submesh : public TGen::Subface {
		public:
			Submesh();
			~Submesh();
			
			void preRender(TGen::Renderer & renderer) const;
			void render(TGen::Renderer & renderer) const;
			
			TGen::VertexData * vb;
			TGen::VertexData * ib;
			TGen::PrimitiveType primitive;
			uint startIndex, indexCount;			
		};
		
	} // !MD5 
} // !TGen

#endif // !_TGEN_RENDERER_MD5SUBMESH_H
