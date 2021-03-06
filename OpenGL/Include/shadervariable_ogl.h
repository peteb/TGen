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
		class ShaderProgram;
		
		enum ShaderVariableType {
			Uniform,
			Attribute,
			VarNone,
		};
		
		class ShaderVariable : public TGen::ShaderVariable {
		private:
			ShaderVariable(GLint location, GLuint program, TGen::OpenGL::ShaderVariableType type);

		public:
			ShaderVariable(); // unlinked
			ShaderVariable(const TGen::OpenGL::ShaderVariable & var);
			~ShaderVariable();

			TGen::ShaderVariable & operator = (int value);
			TGen::ShaderVariable & setInt(int value);
			
			TGen::ShaderVariable & operator = (float value);
			TGen::ShaderVariable & operator = (const TGen::Color & color);
			TGen::ShaderVariable & operator = (const TGen::Vector4 & vector);
			TGen::ShaderVariable & operator = (const TGen::Vector3 & vector);
			TGen::ShaderVariable & operator = (const TGen::Vector2 & vector);
			TGen::ShaderVariable & operator = (const TGen::Matrix4x4 & matrix);
			
			TGen::ShaderVariable & operator = (const std::vector<TGen::Color> & array);
			TGen::ShaderVariable & operator = (const std::vector<TGen::Vector3> & array);

			GLint getInternalID() const;

			friend class TGen::OpenGL::ShaderProgram;
				
		private:
			TGen::OpenGL::ShaderVariableType type;
			GLint location;
			GLuint program;
		};
		
	} // !OpenGL
} // !TGen

#endif // !_TGEN_OPENGL_SHADERVARIABLE_H
