/*
 *  vertexbuffer_ogl.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include <iostream>
#include <tgen_core.h>
#include "prefix_ogl.h"
#include "binder_ogl.h"
#include "shadervariable_ogl.h"

#include "vertexbuffer_ogl.h"
#include "renderer.h"
#include "error.h"

TGen::OpenGL::VertexBuffer::VertexBuffer(TGen::Renderer & creator, const TGen::VertexStructure & vertstruct, uint size, ushort usage, GLuint vboId) 
	: TGen::VertexBuffer(creator, size, usage)
	, vertstruct(vertstruct)
	, vboId(vboId) 
{
}

TGen::OpenGL::VertexBuffer::~VertexBuffer() {
	if (vboId > 0) {
		DEBUG_PRINT("[opengl]: deleting vertex buffer " << vboId);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
		glDeleteBuffersARB(1, reinterpret_cast<GLuint *>(&vboId));
	}	
}

void * TGen::OpenGL::VertexBuffer::lock(ushort flags) {
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

void TGen::OpenGL::VertexBuffer::unlock() {
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboId);
	glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);		
}

void TGen::OpenGL::VertexBuffer::bufferData(const void * data, uint size, void * offset) {
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

GLuint TGen::OpenGL::VertexBuffer::getInternalID() const {
	return vboId;
}

void TGen::OpenGL::VertexBuffer::bindShaderVariable(int id, const TGen::ShaderVariable & var) {
	for (int i = 0; i < vertstruct.getElementCount(); ++i) {
		TGen::VertexElement & element = vertstruct.getElementAt(i);

		if (element.type == TGen::VertexElementAttribute && element.unit == id) {
			std::cout << "FOUND!! at " << i << std::endl;
			element.bound = true;
			element.boundValue = dynamic_cast<const TGen::OpenGL::ShaderVariable &>(var).getInternalID();
		}
	}
}
