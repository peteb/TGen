/*
 *  shaderprogram_ogl.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/28/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "shaderprogram_ogl.h"
#include "shader_ogl.h"
#include "error.h"
#include "shadervariable_ogl.h"
#include "binder_ogl.h"

#include <tgen_core.h>
#include <string>
#include <iostream>

TGen::OpenGL::ShaderProgram::ShaderProgram(TGen::Renderer & creator, GLuint programId) 
	: TGen::ShaderProgram(creator)
	, programId(programId) 
{
}

TGen::OpenGL::ShaderProgram::~ShaderProgram() {
	VariableMap::iterator iter = cachedVariables.begin();
	for (; iter != cachedVariables.end(); ++iter)
		delete iter->second;
}

void TGen::OpenGL::ShaderProgram::attach(TGen::Shader * shader) {
	TGen::OpenGL::Shader * fixedShader = dynamic_cast<TGen::OpenGL::Shader *>(shader);
	
	glAttachShader(programId, fixedShader->getInternalID());	
}

void TGen::OpenGL::ShaderProgram::link() {
	glLinkProgram(programId);
	
	GLint status = 0;
	glGetProgramiv(programId, GL_LINK_STATUS, &status);
	
	char * infoLog = NULL;
	GLint infoLogLength = 0;
	GLsizei charsWritten = 0;
	std::string infoLogString;
	
	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
	
	if (infoLogLength > 0) {
		infoLog = static_cast<char *>(malloc(infoLogLength));
		glGetProgramInfoLog(programId, infoLogLength, &charsWritten, infoLog);
		infoLogString = infoLog;
		free(infoLog);
	}	
	
	if (status != GL_TRUE) {
		throw TGen::RuntimeException("OpenGL::ShaderProgram::Link", "failed to link shaders: \"" + infoLogString + "\"");
	}
	else {
		DEBUG_PRINT("[opengl]: shader program " << programId << " linked: \"" << infoLogString << "\"");
	}

}

TGen::ShaderVariable & TGen::OpenGL::ShaderProgram::getUniform(const std::string & name) {
	GLint location = 0;
	
	VariableMap::iterator iter = cachedVariables.find(name);
	if (iter == cachedVariables.end()) {
		location = glGetUniformLocation(programId, name.c_str());
		if (location == -1)
			throw TGen::RuntimeException("OpenGL::ShaderProgram::getUniform", "variable \"" + name + "\" not found");
		
		TGen::ShaderVariable * newVar = new TGen::OpenGL::ShaderVariable(location, programId, TGen::OpenGL::Uniform);
		cachedVariables.insert(VariableMap::value_type(name, newVar));
		
		return *newVar;
	}
		
	return *iter->second;
}

TGen::ShaderVariable & TGen::OpenGL::ShaderProgram::getAttribute(const std::string & name) {
	GLint location = 0;
	
	VariableMap::iterator iter = cachedVariables.find(name);
	if (iter == cachedVariables.end()) {
		location = glGetAttribLocation(programId, name.c_str());
		if (location == -1)
			throw TGen::RuntimeException("OpenGL::ShaderProgram::getAttribute", "variable \"" + name + "\" not found");
		
		TGen::ShaderVariable * newVar = new TGen::OpenGL::ShaderVariable(location, programId, TGen::OpenGL::Attribute);
		
		cachedVariables.insert(VariableMap::value_type(name, newVar));
		return *newVar;
	}
	
	return *iter->second;	
}

GLuint TGen::OpenGL::ShaderProgram::getInternalID() const {
	return programId;
}
