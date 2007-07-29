/*
 *  vertexbuffer_ogl.h
 *  tgen
 *
 *  Created by Peter Backman on 6/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_OPENGL_VERTEXBUFFER_H
#define _TGEN_OPENGL_VERTEXBUFFER_H

#include "vertexbuffer.h"
#include "vertexstructure.h"
#include "prefix_ogl.h"

namespace TGen {	
	namespace OpenGL {
		class Renderer;
		
		class VertexBuffer : public TGen::VertexBuffer {
		private:
			VertexBuffer(TGen::Renderer & creator, const TGen::VertexStructure & vertstruct, uint size, ushort usage, GLuint vboId);
			
		public:
			~VertexBuffer();
			
			void * Lock(ushort flags);
			void Unlock();
			void BufferData(void * data, uint size, void * offset);
			bool isLocked();
			TGen::VertexStructure & getVertexStructure();
			
			GLuint getInternalID() const;

			friend class TGen::OpenGL::Renderer;
			
		private:
			TGen::VertexStructure vertstruct;
			GLuint vboId;
		};
		
	} // !OpenGL
} // !TGen

#endif // !_TGEN_OPENGL_VERTEXBUFFER_H

