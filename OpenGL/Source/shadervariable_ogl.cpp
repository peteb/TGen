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
#include "vector3.h"
#include "vector2.h"

TGen::OpenGL::ShaderVariable::ShaderVariable(GLint location, GLint program, TGen::OpenGL::ShaderVariableType type) 
	: location(location)
	, program(program)
	, type(type) 
{
}

TGen::OpenGL::ShaderVariable::~ShaderVariable() {
	
}

TGen::ShaderVariable & TGen::OpenGL::ShaderVariable::operator = (int value) {
	return setInt(value);
}

TGen::ShaderVariable & TGen::OpenGL::ShaderVariable::setInt(int value) {
	glUseProgram(program);
	
	if (type == TGen::OpenGL::Uniform)
		glUniform1i(location, value);
	
	return *this;	
}

TGen::ShaderVariable & TGen::OpenGL::ShaderVariable::operator = (const TGen::Color & color) {
	glUseProgram(program);

	if (type == TGen::OpenGL::Uniform)
		glUniform4f(location, color.r, color.g, color.b, color.a);
	else
		glVertexAttrib4f(location, color.r, color.g, color.b, color.a);
	
	return *this;
}

TGen::ShaderVariable & TGen::OpenGL::ShaderVariable::operator = (const TGen::Vector3 & vector) {
	glUseProgram(program);

	if (type == TGen::OpenGL::Uniform)
		glUniform3f(location, vector.x, vector.y, vector.z);
	else
		glVertexAttrib3f(location, vector.x, vector.y, vector.z);
	
	return *this;	
}

TGen::ShaderVariable & TGen::OpenGL::ShaderVariable::operator = (const TGen::Vector2 & vector) {
	glUseProgram(program);

	if (type == TGen::OpenGL::Uniform)
		glUniform2f(location, vector.x, vector.y);
	else
		glVertexAttrib2f(location, vector.x, vector.y);
	
	return *this;	
}

