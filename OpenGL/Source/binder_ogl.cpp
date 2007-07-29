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
#else
#error this platform is not supported
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
	DEBUG_PRINT("[opengl]: binding functions... :(");

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

	glDeleteShader = (DeleteShaderPtr)_GET_EXT("glDeleteShaderARB");
	glAttachShader = (AttachShaderPtr)_GET_EXT("glAttachShaderARB");
	glLinkProgram = (LinkProgramPtr)_GET_EXT("glLinkProgramARB");
	glGetProgramiv = (GetProgramivPtr)_GET_EXT("glGetProgramivARB");
	glGetProgramInfoLog = (GetProgramInfoLogPtr)_GET_EXT("glGetProgramInfoLogARB");
	glGetUniformLocation = (GetUniformLocationPtr)_GET_EXT("glGetUniformLocationARB");
	glGetAttribLocation = (GetAttribLocationPtr)_GET_EXT("glGetAttribLocationARB");
	glUseProgram = (UseProgramPtr)_GET_EXT("glUseProgramARB");

	glUniform1i = (Uniform1iPtr)_GET_EXT("glUniform1iARB");
	glUniform4f = (Uniform4fPtr)_GET_EXT("glUniform4fARB");
	glVertexAttrib4f = (VertexAttrib4fPtr)_GET_EXT("glVertexAttrib4fARB");
	glUniform3f = (Uniform3fPtr)_GET_EXT("glUniform3fARB");
	glVertexAttrib3f = (VertexAttrib3fPtr)_GET_EXT("glVertexAttrib3fARB");
	glUniform2f = (Uniform2fPtr)_GET_EXT("glUniform2fARB");
	glVertexAttrib2f = (VertexAttrib2fPtr)_GET_EXT("glVertexAttrib2fARB");
	glCreateShader = (CreateShaderPtr)_GET_EXT("glCreateShaderARB");
	glShaderSource = (ShaderSourcePtr)_GET_EXT("glShaderSourceARB");
	glCompileShader = (CompileShaderPtr)_GET_EXT("glCompileShaderARB");
	glGetShaderiv = (GetShaderivPtr)_GET_EXT("glGetShaderivARB");
	glGetShaderInfoLog = (GetShaderInfoLogPtr)_GET_EXT("glGetShaderInfoLogARB");
	glCreateProgram = (CreateProgramPtr)_GET_EXT("glCreateProgramARB");

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