/*
 *  typesconverter.cpp
 *  TGen Graphics
 *
 *  Created by Peter Backman on 1/7/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include "typesconverter.h"
#include <tgen_core.h>

TGen::BlendFunc TGen::StringToBlendFunc(const std::string & blend) {
	std::string fixedBlend = TGen::toLower(blend);
	
	if (fixedBlend == "zero" || fixedBlend == "0" || fixedBlend == "gl_zero")
		return TGen::BlendZero;
	else if (fixedBlend == "one" || fixedBlend == "1" || fixedBlend == "gl_one")
		return TGen::BlendOne;
	else if (fixedBlend == "destcolor" || fixedBlend == "dstcolor" || fixedBlend == "gl_dst_color")
		return TGen::BlendDestColor;
	else if (fixedBlend == "oneminusdestcolor" || fixedBlend == "1-dstcolor" || fixedBlend == "gl_one_minus_dst_color")
		return TGen::BlendOneMinusDestColor;
	else if (fixedBlend == "srcalpha" || fixedBlend == "srcalpha" || fixedBlend == "gl_src_alpha")
		return TGen::BlendSourceAlpha;
	else if (fixedBlend == "oneminussrcalpha" || fixedBlend == "1-srcalpha" || fixedBlend == "gl_one_minus_src_alpha")
		return TGen::BlendOneMinusSourceAlpha;
	else if (fixedBlend == "destalpha" || fixedBlend == "dstalpha" || fixedBlend == "gl_dst_alpha")
		return TGen::BlendDestAlpha;
	else if (fixedBlend == "oneminusdestalpha" || fixedBlend == "1-dstalpha" || fixedBlend == "gl_one_minus_dst_alpha")
		return TGen::BlendOneMinusDestAlpha;
	else if (fixedBlend == "srccolor" || fixedBlend == "srccolor" || fixedBlend == "gl_src_color")
		return TGen::BlendSourceColor;
	else if (fixedBlend == "oneminussrccolor" || fixedBlend == "1-srccolor" || fixedBlend == "gl_one_minus_src_color")
		return TGen::BlendOneMinusSourceColor;
	
	throw TGen::RuntimeException("Pass::StringToBlendFunc", "invalid blend func: '" + blend + "'!");		
}

