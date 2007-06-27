/*
 *  texture_ogl.h
 *  tgen
 *
 *  Created by Peter Backman on 6/20/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_OPENGL_TEXTURE_H
#define _TGEN_OPENGL_TEXTURE_H

#include "texture.h"
#include "prefix_ogl.h"

namespace TGen {
	namespace OpenGL {
		class Renderer;
		
		class Texture : public TGen::Texture {
		private:
			Texture(TGen::Renderer & creator, GLuint texId, const TGen::Rectangle & size);

		public:
			~Texture();
		
			friend class TGen::OpenGL::Renderer;
			
			GLuint getInternalID() const;
			
		private:
			GLuint texId;
		};
		
	} // !OpenGL
} // !TGen

#endif // !_TGEN_OPENGL_TEXTURE_H
