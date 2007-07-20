/*
 *  image.h
 *  tgen
 *
 *  Created by Peter Backman on 6/20/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_IMAGE_H
#define _TGEN_IMAGE_H

#include "types.h"
#include "rectangle.h"

namespace TGen {
	enum ImageFormat {
		RGB		= 1,
		RGBA,
		BGR,
		BGRA,
		ARGB,
		RGBA32F,
		RGB32F,
		LUMINANCE,
		LUMINANCE_ALPHA,
		LUMINANCE32F,
		LUMINANCE_ALPHA32F,
		DEPTH16,
		DEPTH24,
		DEPTH32,
		DEPTH,
	};
	
	int ImageFormatComponents(ImageFormat format);
	
	class Image {
	public:
		Image() {}	
		virtual ~Image() {}
		
		virtual TGen::Rectangle getSize() const abstract;
		virtual TGen::ImageFormat getFormat() const abstract;
		virtual TGen::FormatType getComponentFormat() const abstract;		
		virtual const void * getData() const abstract;
	};
	
} // !TGen

#endif // !_TGEN_IMAGE_H
