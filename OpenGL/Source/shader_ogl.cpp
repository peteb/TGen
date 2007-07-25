/*
 *  shader_ogl.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/28/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include <tgen_core.h>
#include "shader_ogl.h"

TGen::OpenGL::Shader::Shader(TGen::Renderer & creator, GLuint shaderId) 
	: TGen::Shader(creator)
	, shaderId(shaderId) 
{
}

TGen::OpenGL::Shader::~Shader() {
	if (shaderId > 0) {
		DEBUG_PRINT("[opengl]: deleting shader " << shaderId);
		glDeleteShader(shaderId);
	}
}

GLuint TGen::OpenGL::Shader::getInternalID() const {
	return shaderId;
}
