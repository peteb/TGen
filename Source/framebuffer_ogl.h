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

namespace TGen {
	namespace OpenGL {
		class Texture;
	
		class FrameBuffer : public TGen::FrameBuffer {
		public:	
			FrameBuffer();
			~FrameBuffer();
		
			void Attach(const TGen::Texture * texture, TGen::FrameBufferAttachment attachpoint);
		};
	
	} // !OpenGL
} // !TGen

#endif // !_TGEN_OPENGL_FRAMEBUFFER_H
