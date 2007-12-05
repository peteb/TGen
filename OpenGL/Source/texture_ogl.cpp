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

TGen::OpenGL::Texture::Texture(TGen::Renderer & creator, GLuint texId, const TGen::Rectangle & size, GLenum target) 
	: TGen::Texture(creator, size)
	, texId(texId) 
	, target(target)
{
}

TGen::OpenGL::Texture::~Texture() {
	if (texId > 0) {
		DEBUG_PRINT("[opengl]: deleting texture " << texId);
		glBindTexture(target, 0);
		glDeleteTextures(1, &texId);
	}	
}

GLuint TGen::OpenGL::Texture::getInternalID() const {
	return texId;
}

void TGen::OpenGL::Texture::setWrapMode(TGen::TextureWrap u, TGen::TextureWrap v) {
	GLenum wrapS = TGen::OpenGL::TgenTextureWrapToOpenGL(u);
	GLenum wrapT = TGen::OpenGL::TgenTextureWrapToOpenGL(v);
	
	if (target == GL_TEXTURE_RECTANGLE_ARB) {
		if (wrapS != GL_CLAMP && wrapS != GL_CLAMP_TO_EDGE && wrapS != GL_CLAMP_TO_BORDER)
			throw TGen::RuntimeException("OpenGL::Texture::setWrapMode", "wrap mode invalid for rectangular texture");
		if (wrapT != GL_CLAMP && wrapT != GL_CLAMP_TO_EDGE && wrapT != GL_CLAMP_TO_BORDER)
			throw TGen::RuntimeException("OpenGL::Texture::setWrapMode", "wrap mode invalid for rectangular texture");
	}
	
	glBindTexture(target, texId);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapT);
}

void TGen::OpenGL::Texture::setFilterMode(TGen::TextureFilter min, TGen::TextureFilter mag) {
	glBindTexture(target, texId);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, TGen::OpenGL::TgenTextureFilterToOpenGL(min));
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, TGen::OpenGL::TgenTextureFilterToOpenGL(mag));
}

