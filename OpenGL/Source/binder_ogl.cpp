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

BindBufferARBPtr glBindBufferARB = NULL;
GenBuffersARBPtr glGenBuffersARB = NULL;
BufferDataARBPtr glBufferDataARB = NULL;
MapBufferARBPtr glMapBufferARB = NULL;
UnmapBufferARBPtr glUnmapBufferARB = NULL;
BufferSubDataARBPtr glBufferSubDataARB = NULL;
DeleteBuffersARBPtr glDeleteBuffersARB = NULL;

ActiveTextureARBPtr glActiveTexture = NULL;
ClientActiveTextureARBPtr glClientActiveTexture = NULL;

DrawRangeElementsEXTPtr glDrawRangeElements = NULL;

DeleteShaderPtr glDeleteShader = NULL;
AttachShaderPtr glAttachShader = NULL;
LinkProgramPtr glLinkProgram = NULL;
GetProgramivPtr glGetProgramiv = NULL;
GetProgramInfoLogPtr glGetProgramInfoLog = NULL;
GetUniformLocationPtr glGetUniformLocation = NULL;
GetAttribLocationPtr glGetAttribLocation = NULL;
UseProgramPtr glUseProgram = NULL;
Uniform1iPtr glUniform1i = NULL;
Uniform4fPtr glUniform4f = NULL;
VertexAttrib4fPtr glVertexAttrib4f = NULL;
Uniform3fPtr glUniform3f = NULL;
VertexAttrib3fPtr glVertexAttrib3f = NULL;
Uniform2fPtr glUniform2f = NULL;
VertexAttrib2fPtr glVertexAttrib2f = NULL;
CreateShaderPtr glCreateShader = NULL;
ShaderSourcePtr glShaderSource = NULL;
CompileShaderPtr glCompileShader = NULL;
GetShaderivPtr glGetShaderiv = NULL;
GetShaderInfoLogPtr glGetShaderInfoLog = NULL;
CreateProgramPtr glCreateProgram = NULL;

DeleteFramebuffersEXTPtr glDeleteFramebuffersEXT = NULL;
GenFramebuffersEXTPtr glGenFramebuffersEXT = NULL;
FramebufferTexture2DEXTPtr glFramebufferTexture2DEXT = NULL;
BindFramebufferEXTPtr glBindFramebufferEXT = NULL;
CheckFramebufferStatusEXTPtr glCheckFramebufferStatusEXT = NULL;

DrawBuffersARBPtr glDrawBuffersARB = NULL;

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
	glCreateShader = (CreateShaderPtr)_GET_EXT("glCreateShader");
	glShaderSource = (ShaderSourcePtr)_GET_EXT("glShaderSource");
	glCompileShader = (CompileShaderPtr)_GET_EXT("glCompileShader");
	glGetShaderiv = (GetShaderivPtr)_GET_EXT("glGetShaderiv");
	glGetShaderInfoLog = (GetShaderInfoLogPtr)_GET_EXT("glGetShaderInfoLog");
	glCreateProgram = (CreateProgramPtr)_GET_EXT("glCreateProgram");

	glDeleteFramebuffersEXT = (DeleteFramebuffersEXTPtr)_GET_EXT("glDeleteFramebuffersEXT");
	glGenFramebuffersEXT = (GenFramebuffersEXTPtr)_GET_EXT("glGenFramebuffersEXT");

	glFramebufferTexture2DEXT = (FramebufferTexture2DEXTPtr)_GET_EXT("glFramebufferTexture2DEXT");
	glBindFramebufferEXT = (BindFramebufferEXTPtr)_GET_EXT("glBindFramebufferEXT");
	glCheckFramebufferStatusEXT = (CheckFramebufferStatusEXTPtr)_GET_EXT("glCheckFramebufferStatusEXT");

	glDrawBuffersARB = (DrawBuffersARBPtr)_GET_EXT("glDrawBuffersARB");

}

#else

void TGen::OpenGL::BindFunctions() {
}

#endif