/*
 *  vertexdata_ogl.cpp
 *  TGen OpenGL
 *
 *  Created by Peter Backman on 12/25/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include <iostream>
#include <tgen_core.h>
#include "prefix_ogl.h"
#include "binder_ogl.h"
#include "shadervariable_ogl.h"

#include "vertexdata_ogl.h"
#include "renderer.h"
#include "error.h"

TGen::OpenGL::VertexData::VertexData(TGen::VertexDataSource & creator, const TGen::VertexStructure & vertstruct, 
												 uint size, ushort usage, GLuint vboId) 
	: TGen::VertexData(creator, size, usage)
	, vertstruct(vertstruct)
	, vboId(vboId) 
{
}

TGen::OpenGL::VertexData::~VertexData() {

}

void * TGen::OpenGL::VertexData::lock(ushort flags) {
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
		throw TGen::RuntimeException("OpenGL::VertexData::Lock", "failed to map vertex data");
	
	return data;
}

void TGen::OpenGL::VertexData::unlock() {
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboId);
	glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);		
}

void TGen::OpenGL::VertexData::bufferData(const void * data, uint size, void * offset) {
	GLenum fixedUsage = usage;
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboId);
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, reinterpret_cast<GLintptrARB>(offset), size, data);	
}

bool TGen::OpenGL::VertexData::isLocked() {
	return false;
}

TGen::VertexStructure & TGen::OpenGL::VertexData::getVertexStructure() {
	return vertstruct;
}

GLuint TGen::OpenGL::VertexData::getInternalID() const {
	return vboId;
}

void TGen::OpenGL::VertexData::bindShaderVariable(int id, const TGen::ShaderVariable & var) {
	for (int i = 0; i < vertstruct.getElementCount(); ++i) {
		TGen::VertexElement & element = vertstruct.getElementAt(i);
		
		if (element.type == TGen::VertexElementAttribute && element.unit == id) {
			element.bound = true;
			element.boundValue = static_cast<const TGen::OpenGL::ShaderVariable &>(var).getInternalID();
		}
	}
}
