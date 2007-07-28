/*
 *  binder_ogl.cpp
 *  tgen
 *
 *  Created by Peter Backman on 7/28/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "binder_ogl.h"
#include "tgen_core.h"

#ifdef _TGEN_OPENGL_NEED_BINDING

#ifdef _PLATFORM_WIN
#include <windows.h>
#define _GET_EXT(x) wglGetProcAddress(x)
#endif


void TGen::OpenGL::BindFunctions() {
	DEBUG_PRINT("[ogl]: binding functions... :(");

	glBindBufferARB = (BindBufferARBPtr)_GET_EXT("glBindBufferARB");
	glGenBuffersARB = (GenBuffersARBPtr)_GET_EXT("glGenBuffersARB");
	glBufferDataARB = (BufferDataARBPtr)_GET_EXT("glBufferDataARB");
	glBufferSubDataARB = (BufferSubDataARBPtr)_GET_EXT("glBufferSubDataARB");
	glMapBufferARB = (MapBufferARBPtr)_GET_EXT("glMapBufferARB");
	glUnmapBufferARB = (UnmapBufferARBPtr)_GET_EXT("glUnmapBufferARB");
	glDeleteBuffersARB = (DeleteBuffersARBPtr)_GET_EXT("glDeleteBuffersARB");

	glActiveTexture = (ActiveTextureARBPtr)_GET_EXT("glActiveTextureARB");
	glClientActiveTexture = (ClientActiveTextureARBPtr)_GET_EXT("glClientActiveTextureARB");

	glDrawRangeElements = (DrawRangeElementsEXTPtr)_GET_EXT("glDrawRangeElementsEXT");

	glDeleteShader = (DeleteShaderPtr)_GET_EXT("glDeleteShader");
	glAttachShader = (AttachShaderPtr)_GET_EXT("glAttachShader");
	glLinkProgram = (LinkProgramPtr)_GET_EXT("glLinkProgram");
	glGetProgramiv = (GetProgramivPtr)_GET_EXT("glGetProgramiv");
	glGetProgramInfoLog = (GetProgramInfoLogPtr)_GET_EXT("glGetProgramInfoLog");
	glGetUniformLocation = (GetUniformLocationPtr)_GET_EXT("glGetUniformLocation");
	glGetAttribLocation = (GetAttribLocationPtr)_GET_EXT("glGetAttribLocation");
	glUseProgram = (UseProgramPtr)_GET_EXT("glUseProgram");

	glUniform1i = (Uniform1iPtr)_GET_EXT("glUniform1i");
	glUniform4f = (Uniform4fPtr)_GET_EXT("glUniform4f");
	glVertexAttrib4f = (VertexAttrib4fPtr)_GET_EXT("glVertexAttrib4f");
	glUniform3f = (Uniform3fPtr)_GET_EXT("glUniform3f");
	glVertexAttrib3f = (VertexAttrib3fPtr)_GET_EXT("glVertexAttrib3f");
	glUniform2f = (Uniform2fPtr)_GET_EXT("glUniform2f");
	glVertexAttrib2f = (VertexAttrib2fPtr)_GET_EXT("glVertexAttrib2f");


}

#else

void TGen::OpenGL::BindFunctions() {
}

#endif