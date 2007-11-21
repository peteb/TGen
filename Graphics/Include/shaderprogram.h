/*
 *  shaderprogram.h
 *  tgen
 *
 *  Created by Peter Backman on 6/28/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_SHADERPROGRAM_H
#define _TGEN_SHADERPROGRAM_H

#include "types.h"
#include <string>

namespace TGen {
	class Renderer;
	class Shader;
	class ShaderVariable;
	
	class ShaderProgram {
	public:	
		ShaderProgram(TGen::Renderer & creator);
		virtual ~ShaderProgram();
		
		virtual void attach(TGen::Shader * shader) abstract;
		virtual void link() abstract;
		virtual ShaderVariable & getUniform(const std::string & name) abstract;
		virtual ShaderVariable & getAttribute(const std::string & name) abstract;
		virtual ShaderVariable * createVariable(const std::string & name) abstract;
		void parseShaders(TGen::Renderer & renderer, char * code);
		
	private:
		TGen::Renderer & creator;
	};
	
} // !TGen

#endif // !_TGEN_SHADERPROGRAM_H
