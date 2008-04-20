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
		class Renderer;
		
		class FrameBuffer : public TGen::FrameBuffer {
		private:
			FrameBuffer(TGen::OpenGL::Renderer & creator, GLuint fboId);

		public:	
			~FrameBuffer();
		
			void attachColor(TGen::Texture * texture);
			void attachDepth(TGen::Texture * texture);
			void attachStencil(TGen::Texture * texture);
			
			void setColorUnit(uint unit, TGen::Texture * texture);
			void setDepthUnit(uint unit, TGen::Texture * texture);
			void setStencilUnit(uint unit, TGen::Texture * texture);
			
			void reset();
			
			void attach(TGen::Texture * texture, TGen::FramebufferAttachment attachpoint);
			GLuint getInternalID() const;
			void setupDrawBuffers();
		
			friend class TGen::OpenGL::Renderer;
			
		private:
			void throwError(GLenum errorCode) const;
			void setAttachPoint(GLenum pointName, TGen::OpenGL::Texture * texture);
			
			std::vector<GLenum> drawBuffers;
			
			GLuint fboId;
			GLint maxColorAttachments;
			
			bool colorUnitsUsed[16];
			bool depthUnitUsed;
			bool stencilUnitUsed;
			
			static GLenum colorPointNames[16];
			int colorPointsTaken, depthPointsTaken, stencilPointsTaken;
			std::vector<GLenum> pointsTaken;
			TGen::OpenGL::Renderer & creator;
		};
	
	} // !OpenGL
} // !TGen

#endif // !_TGEN_OPENGL_FRAMEBUFFER_H
