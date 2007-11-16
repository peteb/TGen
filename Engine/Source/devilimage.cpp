/*
 *  devilimage.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 11/16/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "devilimage.h"

TGen::Engine::DevilImage::DevilImage(ILuint imageName)
	: imageName(imageName)
{
}

TGen::Rectangle TGen::Engine::DevilImage::getSize() const {
	ILuint width, height;
	ilBindImage(imageName);
	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);
	
	return TGen::Rectangle(width, height);	
}

TGen::ImageFormat TGen::Engine::DevilImage::getFormat() const {
	ilBindImage(imageName);
	ILuint type = ilGetInteger(IL_IMAGE_FORMAT);
	
	switch (type) {
		case IL_RGB:
			return TGen::RGB;
			
		case IL_RGBA:
			return TGen::RGBA;
			
		case IL_BGR:
			return TGen::BGR;
			
		case IL_BGRA:
			return TGen::BGRA;		
			
		case IL_LUMINANCE:
			return TGen::LUMINANCE;
	}

	throw TGen::RuntimeException("DevILImage::getFormat", "unknown format: ") << type;
}

TGen::FormatType TGen::Engine::DevilImage::getComponentFormat() const {
	return TGen::TypeUnsignedByte;		
}

const void * TGen::Engine::DevilImage::getData() const {
	ilBindImage(imageName);
	return ilGetData();
}

