/*
 *  framebuffer_ogl.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/26/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include <tgen_core.h>
#include "prefix_ogl.h"
#include "binder_ogl.h"
#include "framebuffer_ogl.h"
#include "texture.h"
#include "framebuffer.h"
#include "texture_ogl.h"
#include "error.h"

TGen::OpenGL::FrameBuffer::FrameBuffer(GLuint fboId) 
	: fboId(fboId)
	, colorPointsTaken(0)
	, depthPointsTaken(0)
	, stencilPointsTaken(0) 
{	
}

TGen::OpenGL::FrameBuffer::~FrameBuffer() {
	if (fboId > 0) {
		DEBUG_PRINT("[opengl]: deleting framebuffer " << fboId);
		glDeleteFramebuffersEXT(1, &fboId);
	}
}

void TGen::OpenGL::FrameBuffer::attach(TGen::Texture * texture, TGen::FramebufferAttachment attachpoint) {
	TGen::OpenGL::Texture * fixedTexture = static_cast<TGen::OpenGL::Texture *>(texture);
	
	GLenum attachment = 0;
	GLint maxBuffers;
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &maxBuffers);
	
	GLenum pointNames[16] = {
		GL_COLOR_ATTACHMENT0_EXT,
		GL_COLOR_ATTACHMENT1_EXT,
		GL_COLOR_ATTACHMENT2_EXT,
		GL_COLOR_ATTACHMENT3_EXT,
		GL_COLOR_ATTACHMENT4_EXT,
		GL_COLOR_ATTACHMENT5_EXT,
		GL_COLOR_ATTACHMENT6_EXT,
		GL_COLOR_ATTACHMENT7_EXT,
		GL_COLOR_ATTACHMENT8_EXT,
		GL_COLOR_ATTACHMENT9_EXT,
		GL_COLOR_ATTACHMENT10_EXT,
		GL_COLOR_ATTACHMENT11_EXT,
		GL_COLOR_ATTACHMENT12_EXT,
		GL_COLOR_ATTACHMENT13_EXT,
		GL_COLOR_ATTACHMENT14_EXT,
		GL_COLOR_ATTACHMENT15_EXT};
	
	switch (attachpoint) {
		case TGen::FramebufferAttachmentColor:
			if (colorPointsTaken < maxBuffers && colorPointsTaken < 16)
				attachment = pointNames[colorPointsTaken++];
			else
				throw TGen::RuntimeException("OpenGL::FrameBuffer::Attach", "can't attach more color textures");
		
			break;
			
		case TGen::FramebufferAttachmentDepth:
			attachment = GL_DEPTH_ATTACHMENT_EXT;
			break;
			
		case TGen::FramebufferAttachmentStencil:
			attachment = GL_STENCIL_ATTACHMENT_EXT;
			break;
		
		default:
			throw TGen::RuntimeException("OpenGL::FrameBuffer::Attach", "invalid attachpoint");
	}
	
	pointsTaken.push_back(attachment);
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachment, GL_TEXTURE_2D, fixedTexture->getInternalID(), 0);
	
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	
	switch (status) {
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
			throw TGen::RuntimeException("OpenGL::FrameBuffer::Attach", "not all attachment points are complete");
			
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
			throw TGen::RuntimeException("OpenGL::FrameBuffer::Attach", "the framebuffer object is empty");
			
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			throw TGen::RuntimeException("OpenGL::FrameBuffer::Attach", "not all attached images have same dimension");
			
		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			throw TGen::RuntimeException("OpenGL::FrameBuffer::Attach", "textures attached don't have same internal format");
			
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
			throw TGen::RuntimeException("OpenGL::FrameBuffer::Attach", "the value of FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_EXT must not be NONE for any color attachment point(s) named by DRAW_BUFFERi.");
			
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
			throw TGen::RuntimeException("OpenGL::FrameBuffer::Attach", "FRAMEBUFFER_ATTACHMENT_OBJET_TYPE_EXT must not be NONE for the color attachment point named by READ_BUFFER.");
			
		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
			throw TGen::RuntimeException("OpenGL::FrameBuffer::Attach", "the attached texture's format is not supported in this framebuffer");
		
		default:
			if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
				throw TGen::RuntimeException("OpenGL::FrameBuffer::Attach", "framebuffer is not complete: unknown error");
	}
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

GLuint TGen::OpenGL::FrameBuffer::getInternalID() const {
	return fboId;
}

/*void TGen::OpenGL::FrameBuffer::getAttachedPoints(std::vector<GLenum> & list) const {
	GLenum pointNames[16] = {GL_COLOR_ATTACHMENT0_EXT,
		GL_COLOR_ATTACHMENT1_EXT,
		GL_COLOR_ATTACHMENT2_EXT,
		GL_COLOR_ATTACHMENT3_EXT,
		GL_COLOR_ATTACHMENT4_EXT,
		GL_COLOR_ATTACHMENT5_EXT,
		GL_COLOR_ATTACHMENT6_EXT,
		GL_COLOR_ATTACHMENT7_EXT,
		GL_COLOR_ATTACHMENT8_EXT,
		GL_COLOR_ATTACHMENT9_EXT,
		GL_COLOR_ATTACHMENT10_EXT,
		GL_COLOR_ATTACHMENT11_EXT,
		GL_COLOR_ATTACHMENT12_EXT,
		GL_COLOR_ATTACHMENT13_EXT,
		GL_COLOR_ATTACHMENT14_EXT,
		GL_COLOR_ATTACHMENT15_EXT};

	for (int i = 0; i < colorPointsTaken; ++i)
		list.push_back(pointNames[i]);
	
	for (int i = 0; i < depthPointsTaken; ++i)
		list.push_back(GL_DEPTH_ATTACHMENT_EXT);
	
	for (int i = 0; i < stencilPointsTaken; ++i)
		list.push_back(GL_STENCIL_ATTACHMENT_EXT);
}
*/

void TGen::OpenGL::FrameBuffer::setupDrawBuffers() {
	glDrawBuffersARB(pointsTaken.size(), &pointsTaken[0]);
}
