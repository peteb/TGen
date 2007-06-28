/*
 *  shaderprogram_ogl.h
 *  tgen
 *
 *  Created by Peter Backman on 6/28/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_OPENGL_SHADERPROGRAM_H
#define _TGEN_OPENGL_SHADERPROGRAM_H

#include "shaderprogram.h"
#include "prefix_ogl.h"
#include <map>

namespace TGen {
	class Shader;
	class ShaderVariable;
	
	namespace OpenGL {
		class ShaderProgram : public TGen::ShaderProgram {
		public:
			ShaderProgram(TGen::Renderer & creator, GLuint programId);
			~ShaderProgram();
			
			void Attach(TGen::Shader * shader);
			void Link();
			TGen::ShaderVariable & getUniform(const std::string & name);
			
			GLuint getInternalID() const;
			
		private:
			typedef std::map<std::string, TGen::ShaderVariable *> VariableMap;
			VariableMap cachedVariables;
			GLuint programId;			
		};
		
	} // !OpenGL
} // !TGen

#endif // !_TGEN_OPENGL_SHADERPROGRAM_H