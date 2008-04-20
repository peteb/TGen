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
#include "renderer_ogl.h"

GLenum TGen::OpenGL::FrameBuffer::colorPointNames[16] = {
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


TGen::OpenGL::FrameBuffer::FrameBuffer(TGen::OpenGL::Renderer & creator, GLuint fboId) 
	: fboId(fboId)
	, creator(creator)
	, colorPointsTaken(0)
	, depthPointsTaken(0)
	, stencilPointsTaken(0) 
	, maxColorAttachments(0)
	, depthUnitUsed(false)
	, stencilUnitUsed(false)
{	
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &maxColorAttachments);
	
	drawBuffers.reserve(20);
	
	for (int i = 0; i < 16; ++i)
		colorUnitsUsed[i] = false;
}

TGen::OpenGL::FrameBuffer::~FrameBuffer() {
	if (fboId > 0) {
		DEBUG_PRINT("[opengl]: deleting framebuffer " << fboId);
		glDeleteFramebuffersEXT(1, &fboId);
	}
}

void TGen::OpenGL::FrameBuffer::attachColor(TGen::Texture * texture) {
	attach(texture, TGen::FramebufferAttachmentColor);
}

void TGen::OpenGL::FrameBuffer::attachDepth(TGen::Texture * texture) {
	attach(texture, TGen::FramebufferAttachmentDepth);
}

void TGen::OpenGL::FrameBuffer::attachStencil(TGen::Texture * texture) {
	attach(texture, TGen::FramebufferAttachmentStencil);
}

void TGen::OpenGL::FrameBuffer::attach(TGen::Texture * texture, TGen::FramebufferAttachment attachpoint) {
	TGen::OpenGL::Texture * fixedTexture = dynamic_cast<TGen::OpenGL::Texture *>(texture);
	
	GLenum attachment = 0;

	
	// TODO: optimize, get rid of glGet. create setColorUnit, setDepthUnit, ..
	
	switch (attachpoint) {
		case TGen::FramebufferAttachmentColor:
			if (colorPointsTaken < maxColorAttachments && colorPointsTaken < 16)
				attachment = colorPointNames[colorPointsTaken++];
			else
				throw TGen::RuntimeException("OpenGL::FrameBuffer::Attach", "can't attach more color textures");

			//pointsTaken.push_back(attachment);
			colorUnitsUsed[colorPointsTaken - 1] = true;
			
			break;
			
		case TGen::FramebufferAttachmentDepth:
			attachment = GL_DEPTH_ATTACHMENT_EXT;
			depthUnitUsed = true;
			break;
			
		case TGen::FramebufferAttachmentStencil:
			attachment = GL_STENCIL_ATTACHMENT_EXT;
			stencilUnitUsed = true;
			break;
		
		default:
			throw TGen::RuntimeException("OpenGL::FrameBuffer::Attach", "invalid attachpoint");
	}
	
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachment, GL_TEXTURE_2D, fixedTexture->getInternalID(), 0);
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	
	if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
		throwError(status);
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);	// TODO: reset framebuffer to previous framebuffer
}

void TGen::OpenGL::FrameBuffer::throwError(GLenum errorCode) const {
	switch (errorCode) {
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
			throw TGen::RuntimeException("OpenGL::FrameBuffer::attach", "not all attachment points are complete");
			
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
			throw TGen::RuntimeException("OpenGL::FrameBuffer::attach", "the framebuffer object is empty");
			
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			throw TGen::RuntimeException("OpenGL::FrameBuffer::attach", "not all attached images have same dimension");
			
		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			throw TGen::RuntimeException("OpenGL::FrameBuffer::attach", "textures attached don't have same internal format");
			
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
			throw TGen::RuntimeException("OpenGL::FrameBuffer::attach", "the value of FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_EXT must not be NONE for any color attachment point(s) named by DRAW_BUFFERi.");
			
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
			throw TGen::RuntimeException("OpenGL::FrameBuffer::attach", "FRAMEBUFFER_ATTACHMENT_OBJET_TYPE_EXT must not be NONE for the color attachment point named by READ_BUFFER.");
			
		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
			throw TGen::RuntimeException("OpenGL::FrameBuffer::attach", "the attached texture's format is not supported in this framebuffer");
			
		default:
			if (errorCode != GL_FRAMEBUFFER_COMPLETE_EXT)
				throw TGen::RuntimeException("OpenGL::FrameBuffer::attach", "framebuffer is not complete: unknown error");
	}	
}

GLuint TGen::OpenGL::FrameBuffer::getInternalID() const {
	return fboId;
}

void TGen::OpenGL::FrameBuffer::setupDrawBuffers() {
	drawBuffers.clear();
	for (int i = 0; i < 16; ++i) {
		if (colorUnitsUsed[i])
			drawBuffers.push_back(colorPointNames[i]);
	}
	
	//if (depthUnitUsed)
	//	drawBuffers.push_back(GL_DEPTH_ATTACHMENT_EXT);
	
	//if (stencilUnitUsed)
	//	drawBuffers.push_back(GL_STENCIL_ATTACHMENT_EXT);
	
	glDrawBuffersARB(drawBuffers.size(), &drawBuffers[0]);
}


void TGen::OpenGL::FrameBuffer::setColorUnit(uint unit, TGen::Texture * texture) {	
	if (unit >= maxColorAttachments || unit >= 16)
		throw TGen::RuntimeException("OpenGL::FrameBuffer::setColorUnit", "unit overrun, maximum number of color attachments is ") << unit;

	if (texture)
		colorUnitsUsed[unit] = true;
	else
		colorUnitsUsed[unit] = false;
	
	setAttachPoint(colorPointNames[unit], static_cast<TGen::OpenGL::Texture *>(texture));
}

void TGen::OpenGL::FrameBuffer::setDepthUnit(uint unit, TGen::Texture * texture) {
	if (unit != 0)
		throw TGen::RuntimeException("OpenGL::FrameBuffer::setDepthUnit", "only unit 0 can be used for depth!");

	if (texture)
		depthUnitUsed = true;
	else
		depthUnitUsed = false;
	
	setAttachPoint(GL_DEPTH_ATTACHMENT_EXT, static_cast<TGen::OpenGL::Texture *>(texture));
}

void TGen::OpenGL::FrameBuffer::setStencilUnit(uint unit, TGen::Texture * texture) {
	if (unit != 0)
		throw TGen::RuntimeException("OpenGL::FrameBuffer::setStencilUnit", "only unit 0 can be used for stencil!");	

	if (texture)
		stencilUnitUsed = true;
	else
		stencilUnitUsed = false;
	
	setAttachPoint(GL_STENCIL_ATTACHMENT_EXT, static_cast<TGen::OpenGL::Texture *>(texture));
}

void TGen::OpenGL::FrameBuffer::setAttachPoint(GLenum pointName, TGen::OpenGL::Texture * texture) {
	TGen::FrameBuffer * boundBuffer = creator.getBoundFramebuffer();
	
	creator.setRenderTarget(this);
	
	if (texture)
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, pointName, GL_TEXTURE_2D, texture->getInternalID(), 0);
	else
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, pointName, GL_TEXTURE_2D, 0, 0);
		
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	
	creator.setRenderTarget(boundBuffer);

	if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
		throwError(status);
}

void TGen::OpenGL::FrameBuffer::reset() {
	TGen::FrameBuffer * boundBuffer = creator.getBoundFramebuffer();
	
	creator.setRenderTarget(this);
	
	for (int i = 0; i < 16; ++i) {
		if (colorUnitsUsed[i]) {
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, colorPointNames[i], GL_TEXTURE_2D, GL_NONE, 0);
			colorUnitsUsed[i] = false;
		}
	}
	
	if (depthUnitUsed) {
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, GL_NONE, 0);
		depthUnitUsed = false;
	}
	
	if (stencilUnitUsed) {
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_TEXTURE_2D, GL_NONE, 0);
		stencilUnitUsed = false;
	}
	
	creator.setRenderTarget(boundBuffer);
}
