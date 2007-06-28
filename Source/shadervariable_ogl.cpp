/*
 *  shadervariable_ogl.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/28/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "shadervariable_ogl.h"
#include "color.h"

TGen::OpenGL::ShaderVariable::ShaderVariable(GLint location) : location(location) {
	
}

TGen::OpenGL::ShaderVariable::~ShaderVariable() {
	
}


TGen::ShaderVariable & TGen::OpenGL::ShaderVariable::operator = (const TGen::Color & color) {
	glUniform4f(location, color.r, color.g, color.b, color.a);
	
	return *this;
}

