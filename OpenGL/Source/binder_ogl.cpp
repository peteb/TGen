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
//#define _GET_EXT(x) wglGetProcAddress(x)

PROC _GET_EXT(const char * ext) {
	PROC ret = wglGetProcAddress(ext);
	if (!ret)
		throw TGen::RuntimeException("_GET_EXT", std::string("failed to load extension ") + ext);

	return ret;
}
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

VertexAttribPointerPtr glVertexAttribPointer = NULL;
EnableVertexAttribArrayPtr glEnableVertexAttribArray = NULL;
DisableVertexAttribArrayPtr glDisableVertexAttribArray = NULL;

UniformMatrix3fvPtr glUniformMatrix3fv = NULL;
UniformMatrix4fvPtr glUniformMatrix4fv = NULL;
Uniform1fPtr glUniform1f = NULL;
VertexAttrib1fPtr glVertexAttrib1f = NULL;

ProgramParameteriEXTPtr glProgramParameteriEXT = NULL;
Uniform4fvARBPtr glUniform4fv = NULL;
VertexAttrib4fvARB glVertexAttrib4fv = NULL;

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

	glDeleteShader = (DeleteShaderPtr)_GET_EXT("glDeleteObjectARB");
	glAttachShader = (AttachShaderPtr)_GET_EXT("glAttachObjectARB");
	glLinkProgram = (LinkProgramPtr)_GET_EXT("glLinkProgramARB");
	glGetProgramiv = (GetProgramivPtr)_GET_EXT("glGetObjectParameterivARB");
	glGetProgramInfoLog = (GetProgramInfoLogPtr)_GET_EXT("glGetInfoLogARB");
	glGetUniformLocation = (GetUniformLocationPtr)_GET_EXT("glGetUniformLocationARB");
	glGetAttribLocation = (GetAttribLocationPtr)_GET_EXT("glGetAttribLocationARB");
	glUseProgram = (UseProgramPtr)_GET_EXT("glUseProgramObjectARB");

	glUniform1f = (Uniform1fPtr)_GET_EXT("glUniform1fARB");
	glUniform1i = (Uniform1iPtr)_GET_EXT("glUniform1iARB");
	glUniform4f = (Uniform4fPtr)_GET_EXT("glUniform4fARB");
	glVertexAttrib1f = (VertexAttrib1fPtr)_GET_EXT("glVertexAttrib1fARB");
	glVertexAttrib4f = (VertexAttrib4fPtr)_GET_EXT("glVertexAttrib4fARB");
	glUniform3f = (Uniform3fPtr)_GET_EXT("glUniform3fARB");
	glVertexAttrib3f = (VertexAttrib3fPtr)_GET_EXT("glVertexAttrib3fARB");
	glUniform2f = (Uniform2fPtr)_GET_EXT("glUniform2fARB");
	glVertexAttrib2f = (VertexAttrib2fPtr)_GET_EXT("glVertexAttrib2fARB");
	glCreateShader = (CreateShaderPtr)_GET_EXT("glCreateShaderObjectARB");
	glShaderSource = (ShaderSourcePtr)_GET_EXT("glShaderSourceARB");
	glCompileShader = (CompileShaderPtr)_GET_EXT("glCompileShaderARB");
	glGetShaderiv = (GetShaderivPtr)_GET_EXT("glGetObjectParameterivARB");
	glGetShaderInfoLog = (GetShaderInfoLogPtr)_GET_EXT("glGetInfoLogARB");
	glCreateProgram = (CreateProgramPtr)_GET_EXT("glCreateProgramObjectARB");
	glVertexAttribPointer = (VertexAttribPointerPtr)_GET_EXT("glVertexAttribPointerARB");
	glEnableVertexAttribArray = (EnableVertexAttribArrayPtr)_GET_EXT("glEnableVertexAttribArrayARB");
	glDisableVertexAttribArray = (DisableVertexAttribArrayPtr)_GET_EXT("glDisableVertexAttribArrayARB");

	glUniformMatrix3fv = (UniformMatrix3fvPtr)_GET_EXT("glUniformMatrix3fvARB");
	glUniformMatrix4fv = (UniformMatrix4fvPtr)_GET_EXT("glUniformMatrix4fvARB");

	glDeleteFramebuffersEXT = (DeleteFramebuffersEXTPtr)_GET_EXT("glDeleteFramebuffersEXT");
	glGenFramebuffersEXT = (GenFramebuffersEXTPtr)_GET_EXT("glGenFramebuffersEXT");

	glFramebufferTexture2DEXT = (FramebufferTexture2DEXTPtr)_GET_EXT("glFramebufferTexture2DEXT");
	glBindFramebufferEXT = (BindFramebufferEXTPtr)_GET_EXT("glBindFramebufferEXT");
	glCheckFramebufferStatusEXT = (CheckFramebufferStatusEXTPtr)_GET_EXT("glCheckFramebufferStatusEXT");

	glDrawBuffersARB = (DrawBuffersARBPtr)_GET_EXT("glDrawBuffersARB");

	glProgramParameteriEXT = (ProgramParameteriEXTPtr)_GET_EXT("glProgramParameteriEXT");
	glUniform4fv = (Uniform4fvARBPtr)_GET_EXT("glUniform4fvARB");
	glVertexAttrib4fv = (VertexAttrib4fvARB)_GET_EXT("glVertexAttrib4fvARB");

}

#else

void TGen::OpenGL::BindFunctions() {
}

#endif