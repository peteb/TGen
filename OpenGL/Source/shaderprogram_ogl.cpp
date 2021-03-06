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
#include "types_converter_ogl.h"

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
	updateProperties();
	
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
	
	int newlinePos = infoLogString.rfind("\n");
	if (newlinePos != std::string::npos) {
		if (infoLogString.size() - newlinePos < 3)
			infoLogString = infoLogString.substr(0, newlinePos);
	}
	
	if (status != GL_TRUE) {
		throw TGen::RuntimeException("OpenGL::ShaderProgram::Link", "failed to link shaders: \"" + infoLogString + "\"");
	}
	else {
		DEBUG_PRINT("[opengl]: shader program " << programId << " linked: \"" << infoLogString << "\"");
	}

}

void TGen::OpenGL::ShaderProgram::updateProperties() {
	int temp;
	glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT, &temp);

	for (PropertyMap::iterator iter = properties.begin(); iter != properties.end(); ++iter) {
		if (iter->first == "geom_input") {
			glProgramParameteriEXT(programId, GL_GEOMETRY_INPUT_TYPE_EXT, TGen::OpenGL::TgenPrimitiveToOpenGL(TGen::stringToPrimitive(iter->second)));			
		}
		else if (iter->first == "geom_output") {
			glProgramParameteriEXT(programId, GL_GEOMETRY_OUTPUT_TYPE_EXT, TGen::OpenGL::TgenPrimitiveToOpenGL(TGen::stringToPrimitive(iter->second)));
		}		
		else if (iter->first == "geom_output_vertices") {
			if (TGen::toLower(iter->second) == "max") {
				glProgramParameteriEXT(programId, GL_GEOMETRY_VERTICES_OUT_EXT, temp);					
			}
			else {
				int numVerts = TGen::lexical_cast<int>(iter->second);
				glProgramParameteriEXT(programId, GL_GEOMETRY_VERTICES_OUT_EXT, numVerts);	
			}
		}
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

TGen::ShaderVariable * TGen::OpenGL::ShaderProgram::createVariable(const std::string & name) {
	TGen::ShaderVariable * ret = NULL;
	try {
		ret = new TGen::OpenGL::ShaderVariable(dynamic_cast<TGen::OpenGL::ShaderVariable &>(getUniform(name)));
	}
	catch (...) {
		try {
			ret = new TGen::OpenGL::ShaderVariable(dynamic_cast<TGen::OpenGL::ShaderVariable &>(getAttribute(name)));
		}
		catch (...) {
			throw TGen::RuntimeException("OpenGL::ShaderProgram::createVariable", "couldn't find uniform/attribute '" + name + "'!");
		}
	}
	
	return ret;
}

GLuint TGen::OpenGL::ShaderProgram::getInternalID() const {
	return programId;
}
