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
#include "vector4.h"
#include "vector3.h"
#include "vector2.h"
#include "matrix4x4.h"
#include "binder_ogl.h"
#include "error.h"

TGen::OpenGL::ShaderVariable::ShaderVariable(GLint location, GLuint program, TGen::OpenGL::ShaderVariableType type) 
	: location(location)
	, program(program)
	, type(type) 
{
}

TGen::OpenGL::ShaderVariable::ShaderVariable()
	: location(-1)
	, type(TGen::OpenGL::VarNone)
	, program(-1)
{
}

TGen::OpenGL::ShaderVariable::ShaderVariable(const TGen::OpenGL::ShaderVariable & var) 
	: location(var.location)
	, type(var.type)
	, program(var.program)
{
	
}

TGen::OpenGL::ShaderVariable::~ShaderVariable() {
	// varje uniform i pass sparas som varname + namnet den binds till
	// när man länkat in shader i pass:
	// bind alla uniforms till variabler i shadern
	// sen har man kvar namn ($texelsize) -> shadervar
	// löses upp när man initerar varje pass... men kan bli tungt att köra strings direkt
	// typ... PassShaderVarUpdater:   void updateVar(ShaderVar & var, string name)
	
}

TGen::ShaderVariable & TGen::OpenGL::ShaderVariable::operator = (int value) {
	return setInt(value);
}

TGen::ShaderVariable & TGen::OpenGL::ShaderVariable::setInt(int value) {
	glUseProgram(program);
	
	if (type == TGen::OpenGL::Uniform)
		glUniform1i(location, value);
	else
		glVertexAttrib1f(location, float(value));
	
	return *this;	
}

TGen::ShaderVariable & TGen::OpenGL::ShaderVariable::operator = (float value) {
	glUseProgram(program);
	
	if (type == TGen::OpenGL::Uniform)
		glUniform1f(location, value);
	else
		glVertexAttrib1f(location, value);
	
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

TGen::ShaderVariable & TGen::OpenGL::ShaderVariable::operator = (const TGen::Matrix4x4 & matrix) {
	glUseProgram(program);
	
	if (type == TGen::OpenGL::Uniform)
		glUniformMatrix3fv(location, 1, false, reinterpret_cast<const GLfloat *>(&matrix.elements));		// TODO: if sizeof(scalar) != sizeof(GLfloat) we have a huge problem
	else
		throw TGen::RuntimeException("OpenGL::ShaderVariable", "can't set attribute to matrix");
	
		//glVertexAttrib2f(location, vector.x, vector.y);
	
	return *this;		
}

TGen::ShaderVariable & TGen::OpenGL::ShaderVariable::operator = (const TGen::Vector4 & vector) {
	glUseProgram(program);
	
	if (type == TGen::OpenGL::Uniform)
		glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
	else
		glVertexAttrib4f(location, vector.x, vector.y, vector.z, vector.w);
	
	return *this;
}

GLint TGen::OpenGL::ShaderVariable::getInternalID() const {
	return location;
}

