/*
 *  vertexbuffer_ogl.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "vertexbuffer_ogl.h"
#include "platform.h"
#include "renderer.h"
#include "error.h"

#ifdef _PLATFORM_OSX
	#include <OpenGL/OpenGL.h>
	#include <GLUT/GLUT.h>
#else
	#include <GL/GL.h>
#endif


#include <iostream>



TGen::OpenGL::VertexBuffer::VertexBuffer(TGen::Renderer & creator, const TGen::VertexStructure & vertstruct, uint size, ushort usage, uint vboId) : 
	TGen::VertexBuffer(creator, size, usage), vertstruct(vertstruct), vboId(vboId) {}

TGen::OpenGL::VertexBuffer::~VertexBuffer() {
	if (vboId > 0) {
		std::cout << "[opengl]: deleting vertex buffer " << vboId << std::endl;
		glDeleteBuffersARB(1, reinterpret_cast<GLuint *>(&vboId));
	}	
}

void * TGen::OpenGL::VertexBuffer::Lock(ushort flags) {
	
	return 0;
}

void TGen::OpenGL::VertexBuffer::Unlock() {
	
}

void TGen::OpenGL::VertexBuffer::BufferData(void * data, uint size, void * offset) {
	GLenum fixedUsage = 0;
	if (usage == TGen::UsageDynamic)
		fixedUsage = GL_STREAM_DRAW_ARB;
	else if (usage == TGen::UsageStatic)
		fixedUsage = GL_STATIC_DRAW_ARB;
	else
		throw TGen::NotImplemented("OpenGL::VertexBuffer::Lock", "invalid usage: ") << usage;
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboId);
	
	if (offset != 0)
		glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, reinterpret_cast<GLintptrARB>(offset), size, data);	
	else
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, static_cast<GLsizeiptr>(size), data, fixedUsage);	
}

bool TGen::OpenGL::VertexBuffer::isLocked() {
	return false;
}

TGen::VertexStructure & TGen::OpenGL::VertexBuffer::getVertexStructure() {
	return vertstruct;
}