/*
 *  renderer_ogl.h
 *  tgen
 *
 *  Created by Peter Backman on 6/12/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_OPENGL_RENDERER_H
#define _TGEN_OPENGL_RENDERER_H

#include "renderer.h"

namespace TGen {
	class VertexStructure;
	class VertexBuffer;
	
	namespace OpenGL {
		class VertexStructure;
		
		class Renderer : public TGen::Renderer {
		public:
			Renderer();
			~Renderer();
			
			void setClearColor(const TGen::Color & color);
			void setViewport(const TGen::Rectangle & viewport);
			
			void Clear(ushort buffers);
			
			
			TGen::VertexBuffer * CreateVertexBuffer(const TGen::VertexStructure & vertstruct, uint size, ushort usage);
		};
		
	} // !OpenGL	
} // !TGen

#endif // !_TGEN_OPENGL_RENDERER_H
