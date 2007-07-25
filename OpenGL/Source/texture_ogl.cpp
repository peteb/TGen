/*
 *  texture_ogl.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/20/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include <tgen_core.h>
#include <iostream>
#include "texture_ogl.h"
#include "types_converter_ogl.h"

TGen::OpenGL::Texture::Texture(TGen::Renderer & creator, GLuint texId, const TGen::Rectangle & size) 
	: TGen::Texture(creator, size)
	, texId(texId) 
{
}

TGen::OpenGL::Texture::~Texture() {
	if (texId > 0) {
		DEBUG_PRINT("[opengl]: deleting texture " << texId);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &texId);
	}	
}

GLuint TGen::OpenGL::Texture::getInternalID() const {
	return texId;
}

void TGen::OpenGL::Texture::setWrapMode(TGen::TextureWrap u, TGen::TextureWrap v) {
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, TGen::OpenGL::TgenTextureWrapToOpenGL(u));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TGen::OpenGL::TgenTextureWrapToOpenGL(v));
}

void TGen::OpenGL::Texture::setFilterMode(TGen::TextureFilter min, TGen::TextureFilter mag) {
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TGen::OpenGL::TgenTextureFilterToOpenGL(min));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TGen::OpenGL::TgenTextureFilterToOpenGL(mag));
}

