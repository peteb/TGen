/*
 *  texture.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/20/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "texture.h"

TGen::Texture::Texture(TGen::Renderer & creator, const TGen::Rectangle & size) 
	: creator(creator)
	, size(size) 
{

}
