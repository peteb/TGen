/*
 *  image.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/20/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "image.h"

TGen::Image::Image() {

}

TGen::Image::~Image() {

}

int TGen::ImageFormatComponents(ImageFormat format) {
	switch (format) {
		case TGen::BGR:
		case TGen::RGB:
			return 3;
		
		case TGen::BGRA:
		case TGen::RGBA:
			return 4;
			
		case LUMINANCE:
			return 1;
			
		case LUMINANCE_ALPHA:
			return 2;			
	}
	
	return 0;
}

