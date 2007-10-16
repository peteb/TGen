/*
 *  font.h
 *  TGen Graphics
 *
 *  Created by Peter Backman on 10/9/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_GRAPHICS_FONT_H
#define _TGEN_GRAPHICS_FONT_H

#include <string>

namespace TGen {
	class FontText;
	class Material;
	
	class Font {
	public:
		Font(TGen::Material * material);
		
		TGen::FontText * createText(const std::string & text);
		
	private:
		TGen::Material * material;
	};
} // !TGen

#endif // !_TGEN_GRAPHICS_FONT_H
