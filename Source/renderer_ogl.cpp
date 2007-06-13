/*
 *  renderer_ogl.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/12/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "renderer_ogl.h"
#include "platform.h"
#include "vertexstructure.h"
#include "vertexbuffer_ogl.h"

#ifdef _PLATFORM_OSX
	#include <OpenGL/OpenGL.h>
	#include <GLUT/GLUT.h>
#else
	#include <GL/GL.h>
#endif

TGen::OpenGL::Renderer::Renderer() {}
TGen::OpenGL::Renderer::~Renderer() {}

void TGen::OpenGL::Renderer::setClearColor(const TGen::Color & color) {
	TGen::Renderer::setClearColor(color);

	glClearColor(color.r, color.g, color.g, color.a);
}

void TGen::OpenGL::Renderer::setViewport(const TGen::Rectangle & viewport) {
	TGen::Vector2 upperLeft = viewport.getUpperLeft();
	
	glViewport(upperLeft.x, upperLeft.y, viewport.width, viewport.height);	// NOTE: ogl origo is lower-left corner
}

void TGen::OpenGL::Renderer::Clear(ushort buffers) {
	GLbitfield fixed = 0;
	
	if (buffers & TGen::ColorBuffer)
		fixed |= GL_COLOR_BUFFER_BIT;
	if (buffers & TGen::DepthBuffer)
		fixed |= GL_DEPTH_BUFFER_BIT;
	if (buffers & TGen::StencilBuffer)
		fixed |= GL_STENCIL_BUFFER_BIT;
	
	glClear(fixed);
}

TGen::VertexBuffer * TGen::OpenGL::Renderer::CreateVertexBuffer(const TGen::VertexStructure & vertstruct, uint size, ushort usage) {
	return new TGen::OpenGL::VertexBuffer(*this, vertstruct, size, usage);
}
