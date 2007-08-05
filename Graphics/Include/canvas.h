/*
 *  canvas.h
 *  tgen
 *
 *  Created by Peter Backman on 6/20/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_CANVAS_H
#define _TGEN_CANVAS_H

#include "types.h"
#include "rectangle.h"
#include "image.h"

namespace TGen {
	class Color;
	
	class Canvas : public TGen::Image {
	public:
		Canvas(const TGen::Rectangle & size, TGen::ImageFormat format = TGen::RGBA, TGen::FormatType componentFormat = TGen::TypeUnsignedByte);
		~Canvas();
		
		TGen::Rectangle getSize() const;
		TGen::ImageFormat getFormat() const;
		TGen::FormatType getComponentFormat() const;
		const void * getData() const;
		
		void fill(const TGen::Color & color);
		void setPixel(const TGen::Vector2 & pos, const TGen::Color & color);
		void drawLine(const TGen::Vector2 & start, const TGen::Vector2 & end, const TGen::Color & color);
		
		
	private:
		void setPixelRaw(const TGen::Vector2 & pos, void * data, uint bpp);
		
		TGen::Rectangle size;
		TGen::ImageFormat format;
		TGen::FormatType componentFormat;
		void * data;
	};
	
} // !TGen

#endif // !_TGEN_CANVAS_H
