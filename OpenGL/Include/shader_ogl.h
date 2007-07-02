/*
 *  shader_ogl.h
 *  tgen
 *
 *  Created by Peter Backman on 6/28/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_OPENGL_SHADER_H
#define _TGEN_OPENGL_SHADER_H

#include "shader.h"
#include "prefix_ogl.h"

namespace TGen {
	class Renderer;
	
	namespace OpenGL {
		class Shader : public TGen::Shader {
		public:	
			Shader(TGen::Renderer & creator, GLuint shaderId);
			~Shader();
			
			GLuint getInternalID() const;
			
		private:
		   GLuint shaderId;
		};
		
	} // !OpenGL
} // !TGen

#endif // !_TGEN_OPENGL_SHADER_H
