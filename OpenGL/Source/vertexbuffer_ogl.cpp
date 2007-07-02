/*
 *  vertexbuffer_ogl.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "prefix_ogl.h"
#include "vertexbuffer_ogl.h"
#include "renderer.h"
#include "error.h"

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
	GLenum fixedAccess = 0;
	
	if (flags & TGen::LockReadWrite)
		fixedAccess = GL_READ_WRITE_ARB;
	else if (flags & TGen::LockWrite)
		fixedAccess = GL_WRITE_ONLY_ARB;
	else if (flags & TGen::LockRead)
		fixedAccess = GL_READ_ONLY_ARB;
	
	void * data = NULL;
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboId);
	
	if (flags & TGen::LockDiscard)
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, size, NULL, usage);
	
	data = glMapBufferARB(GL_ARRAY_BUFFER_ARB, fixedAccess);
	
	if (!data)
		throw TGen::RuntimeException("OpenGL::VertexBuffer::Lock", "failed to map vertex buffer");
	
	return data;
}

void TGen::OpenGL::VertexBuffer::Unlock() {
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboId);
	glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);		
}

void TGen::OpenGL::VertexBuffer::BufferData(void * data, uint size, void * offset) {
	GLenum fixedUsage = usage;

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