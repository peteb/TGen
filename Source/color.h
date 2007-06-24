/*
 *  color.h
 *  tgen
 *
 *  Created by Peter Backman on 6/12/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_COLOR_H
#define _TGEN_COLOR_H

#include <string>
#include "types.h"
#include "image.h"

namespace TGen {
	class Color {
	public:
		Color(scalar r, scalar g, scalar b, scalar a = 1.0f);
		Color(const TGen::Color & color = TGen::Color::Identity);
		
		operator std::string() const;
		TGen::Color & operator = (const TGen::Color & color);
		TGen::Color & operator *= (const TGen::Color & color);
		TGen::Color & operator *= (scalar scalar);
		
		scalar getAverage() const;
		void getFormatted(TGen::ImageFormat format, TGen::FormatType componentFormat, void * store) const;
		void getValueFormatted(TGen::FormatType format, float value, void * store) const;
		
		static Color Identity, White, Black, Red, Green, Blue, Purple;
		scalar r, g, b, a;		
	};
	
} // !TGen

#endif // !_TGEN_COLOR_H
