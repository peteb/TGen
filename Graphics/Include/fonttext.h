/*
 *  fonttext.h
 *  TGen Graphics
 *
 *  Created by Peter Backman on 10/9/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_GRAPHICS_FONTTEXT_H
#define _TGEN_GRAPHICS_FONTTEXT_H

#include "color.h"
#include <tgen_core.h>

namespace TGen {
	class Renderer;
	
	enum FontTextFormat {
		// color osv?
	};
	
	class FontText {
	public:
		FontText(const std::string & text, const TGen::Color & color, bool dynamic);
		
		void render(TGen::Renderer & renderer, const TGen::Vector3 & position);
		
	private:
		std::string text;
		TGen::Color color, shadowColor;
		FontTextFormat format;
		bool shadow, dynamic;
		scalar size;
	};
	
	
} // !TGen

#endif // !_TGEN_GRAPHICS_FONTTEXT_H
