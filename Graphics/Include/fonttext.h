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
	enum FontTextFormat {
		
	};
	
	class FontText {
	public:
		
		
	private:
		TGen::Color color, shadowColor;
		FontTextFormat format;
		bool shadow;
		scalar size;
	};
	
	
} // !TGen

#endif // !_TGEN_GRAPHICS_FONTTEXT_H
