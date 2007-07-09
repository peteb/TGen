/*
 *  devil.cpp
 *  The Place
 *
 *  Created by Peter Backman on 7/9/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "devil.h"

DevilImage::DevilImage(ILuint imageName) : imageName(imageName) {
	
}

DevilImage::~DevilImage() {
	//ilDeleteImages(1, &imageName); // we're deleting in level above
}


TGen::Rectangle DevilImage::getSize() const {
	ILuint width, height;
	ilBindImage(imageName);
	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);
	
	return TGen::Rectangle(width, height);	
}

TGen::ImageFormat DevilImage::getFormat() const {
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
	}
	
	throw TGen::RuntimeException("DevilImage::getFormat", "unknown format: ") << type;	
}

TGen::FormatType DevilImage::getComponentFormat() const {
	return TGen::TypeUnsignedByte;	
}

const void * DevilImage::getData() const {
	ilBindImage(imageName);
	return ilGetData();	
}
