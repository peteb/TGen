/*
 *  indexbuffer_ogl.h
 *  tgen
 *
 *  Created by Peter Backman on 6/19/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_OPENGL_INDEXBUFFER_H
#define _TGEN_OPENGL_INDEXBUFFER_H

#include "indexbuffer.h"
#include "vertexstructure.h"

namespace TGen {	
	namespace OpenGL {
		class Renderer;
		
		class IndexBuffer : public TGen::IndexBuffer {
		private:
			IndexBuffer(TGen::Renderer & creator, const TGen::VertexStructure & vertstruct, uint size, ushort usage, uint vboId);
			
		public:
			~IndexBuffer();
			
			void * lock(ushort flags);
			void unlock();
			void bufferData(void * data, uint size, void * offset);
			bool isLocked();
			TGen::VertexStructure & getVertexStructure();
			
			friend class TGen::OpenGL::Renderer;
			
		private:
			TGen::VertexStructure vertstruct;
			uint vboId;
		};
		
	} // !OpenGL
} // !TGen

#endif // !_TGEN_OPENGL_VERTEXBUFFER_H
