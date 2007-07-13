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
