/*
 *  texture_ogl.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/20/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "texture_ogl.h"
#include <iostream>

TGen::OpenGL::Texture::Texture(TGen::Renderer & creator, GLuint texId, const TGen::Rectangle & size) 
	: TGen::Texture(creator, size), texId(texId) 
{
	
}

TGen::OpenGL::Texture::~Texture() {
	if (texId > 0) {
		std::cout << "[opengl]: deleting texture " << texId << std::endl;
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &texId);
	}	
}

GLuint TGen::OpenGL::Texture::getInternalID() const {
	return texId;
}

void TGen::OpenGL::Texture::setWrap(TGen::TextureWrap u, TGen::TextureWrap v) {
	GLint fixedU = 0, fixedV = 0;
	// TODO: fix converter
	
	switch (u) {
		case TGen::TextureWrapClamp:
			fixedU = GL_CLAMP;
			break;
			
		case TGen::TextureWrapClampToEdge:
			fixedU = GL_CLAMP_TO_EDGE;
			break;
			
		case TGen::TextureWrapRepeat:
			fixedU = GL_REPEAT;
			break;
	}

	switch (v) {
		case TGen::TextureWrapClamp:
			fixedV = GL_CLAMP;
			break;
			
		case TGen::TextureWrapClampToEdge:
			fixedV = GL_CLAMP_TO_EDGE;
			break;
			
		case TGen::TextureWrapRepeat:
			fixedV = GL_REPEAT;
			break;
	}
	
	std::cout << "setting wrap " << std::endl;
	if (fixedU == TGen::TextureWrapClamp)
		std::cout << "CLAMP!!!!" << std::endl;
	
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, fixedU);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, fixedV);

}
