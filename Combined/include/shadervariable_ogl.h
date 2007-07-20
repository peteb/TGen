/*
 *  shadervariable_ogl.h
 *  tgen
 *
 *  Created by Peter Backman on 6/28/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_OPENGL_SHADERVARIABLE_H
#define _TGEN_OPENGL_SHADERVARIABLE_H

#include "shadervariable.h"
#include "prefix_ogl.h"

namespace TGen {
	class Color;
	
	namespace OpenGL {
		enum ShaderVariableType {
			Uniform,
			Attribute,
		};
		
		class ShaderVariable : public TGen::ShaderVariable {
		public:
			ShaderVariable(GLint location, TGen::OpenGL::ShaderVariableType type);
			~ShaderVariable();
			
			TGen::ShaderVariable & operator = (const TGen::Color & color);
			TGen::ShaderVariable & operator = (const TGen::Vector3 & vector);
			TGen::ShaderVariable & operator = (const TGen::Vector2 & vector);
		
			
		private:
			TGen::OpenGL::ShaderVariableType type;
			GLint location;
		};
		
	} // !OpenGL
} // !TGen

#endif // !_TGEN_OPENGL_SHADERVARIABLE_H
