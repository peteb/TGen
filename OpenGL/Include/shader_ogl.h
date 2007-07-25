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
		class Renderer;
		
		class Shader : public TGen::Shader {
		private:
			Shader(TGen::Renderer & creator, GLuint shaderId);

		public:	
			~Shader();
			
			GLuint getInternalID() const;
		
			friend class TGen::OpenGL::Renderer;
			
		private:
		   GLuint shaderId;
		};
		
	} // !OpenGL
} // !TGen

#endif // !_TGEN_OPENGL_SHADER_H
