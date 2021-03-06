/*
 *  indexbuffer_ogl.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/19/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "prefix_ogl.h"
#include "binder_ogl.h"
#include "indexbuffer_ogl.h"
#include "renderer.h"
#include "error.h"
#include <tgen_core.h>
#include <iostream>

TGen::OpenGL::IndexBuffer::IndexBuffer(TGen::Renderer & creator, const TGen::VertexStructure & vertstruct, uint size, ushort usage, uint vboId) 
	: TGen::IndexBuffer(creator, size, usage)
	, vertstruct(vertstruct), vboId(vboId) 
{

}

TGen::OpenGL::IndexBuffer::~IndexBuffer() {
	if (vboId > 0) {
		DEBUG_PRINT("[opengl]: deleting index buffer " << vboId);
		glDeleteBuffersARB(1, reinterpret_cast<GLuint *>(&vboId));
	}	
}

void * TGen::OpenGL::IndexBuffer::lock(ushort flags) {
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
		throw TGen::RuntimeException("OpenGL::IndexBuffer::Lock", "failed to map vertex buffer");

	return data;
}

// TODO: glGetError()

void TGen::OpenGL::IndexBuffer::unlock() {
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboId);
	glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);	
}

void TGen::OpenGL::IndexBuffer::bufferData(const void * data, uint size, void * offset) {
	GLenum fixedUsage = usage;	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboId);
	
	if (offset != 0)
		glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, reinterpret_cast<GLintptrARB>(offset), size, data);	
	else
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, static_cast<GLsizeiptr>(size), data, fixedUsage);	
}

bool TGen::OpenGL::IndexBuffer::isLocked() {
	return false;
}

TGen::VertexStructure & TGen::OpenGL::IndexBuffer::getVertexStructure() {
	return vertstruct;
}

GLuint TGen::OpenGL::IndexBuffer::getInternalID() const {
	return vboId;
}
