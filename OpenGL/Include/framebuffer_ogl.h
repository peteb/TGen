/*
 *  framebuffer_ogl.h
 *  tgen
 *
 *  Created by Peter Backman on 6/26/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_OPENGL_FRAMEBUFFER_H
#define _TGEN_OPENGL_FRAMEBUFFER_H

#include "framebuffer.h"
#include "prefix_ogl.h"
#include <vector>

namespace TGen {
	namespace OpenGL {
		class Texture;
	
		class FrameBuffer : public TGen::FrameBuffer {
		public:	
			FrameBuffer(GLuint fboId);
			~FrameBuffer();
		
			void Attach(TGen::Texture * texture, TGen::FrameBufferAttachment attachpoint);
			GLuint getInternalID() const;
			void SetupDrawBuffers();
			
		private:
			GLuint fboId;
			int colorPointsTaken, depthPointsTaken, stencilPointsTaken;
			std::vector<GLenum> pointsTaken;
		};
	
	} // !OpenGL
} // !TGen

#endif // !_TGEN_OPENGL_FRAMEBUFFER_H