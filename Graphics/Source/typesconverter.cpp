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
	TGen::BlendFunc ret;
	std::string fixedBlend = TGen::toLower(blend);
	
	if (fixedBlend == "zero" || fixedBlend == "0" || fixedBlend == "gl_zero")
		ret = TGen::BlendZero;
	else if (fixedBlend == "one" || fixedBlend == "1" || fixedBlend == "gl_one")
		ret = TGen::BlendOne;
	else if (fixedBlend == "destcolor" || fixedBlend == "dstcolor" || fixedBlend == "gl_dst_color")
		ret = TGen::BlendDestColor;
	else if (fixedBlend == "oneminusdestcolor" || fixedBlend == "1-dstcolor" || fixedBlend == "gl_one_minus_dst_color")
		ret = TGen::BlendOneMinusDestColor;
	else if (fixedBlend == "srcalpha" || fixedBlend == "srcalpha" || fixedBlend == "gl_src_alpha")
		ret = TGen::BlendSourceAlpha;
	else if (fixedBlend == "oneminussrcalpha" || fixedBlend == "1-srcalpha" || fixedBlend == "gl_one_minus_src_alpha")
		ret = TGen::BlendOneMinusSourceAlpha;
	else if (fixedBlend == "destalpha" || fixedBlend == "dstalpha" || fixedBlend == "gl_dst_alpha")
		ret = TGen::BlendDestAlpha;
	else if (fixedBlend == "oneminusdestalpha" || fixedBlend == "1-dstalpha" || fixedBlend == "gl_one_minus_dst_alpha")
		ret = TGen::BlendOneMinusDestAlpha;
	else if (fixedBlend == "srccolor" || fixedBlend == "srccolor" || fixedBlend == "gl_src_color")
		ret = TGen::BlendSourceColor;
	else if (fixedBlend == "oneminussrccolor" || fixedBlend == "1-srccolor" || fixedBlend == "gl_one_minus_src_color")
		ret = TGen::BlendOneMinusSourceColor;
	else
		throw TGen::RuntimeException("StringToBlendFunc", "invalid blend func: '" + blend + "'!");		
	
	return ret;
}

TGen::CompareFunc StringToCompareFunc(const std::string & func) {
	TGen::CompareFunc ret;
	std::string fixedFunc = TGen::toLower(func);
	
	if (fixedFunc == "never")
		ret = TGen::CompareNever;
	else if (fixedFunc == "less")
		ret = TGen::CompareLess;
	else if (fixedFunc == "equal")
		ret = TGen::CompareEqual;
	else if (fixedFunc == "lequal")
		ret = TGen::CompareLessOrEqual;
	else if (fixedFunc == "greater")
		ret = TGen::CompareGreater;
	else if (fixedFunc == "nequal")
		ret = TGen::CompareNotEqual;
	else if (fixedFunc == "gequal")
		ret = TGen::CompareGreaterOrEqual;
	else if (fixedFunc == "always")
		ret = TGen::CompareAlways;
	else
		throw TGen::RuntimeException("StringToCompareFunc", "invalid compare function: '" + fixedFunc + "'!");
	
	return ret;	
}

TGen::PolygonFaceMode StringToPolygonFaceMode(const std::string & mode) {
	TGen::PolygonFaceMode ret;
	std::string fixedMode = TGen::toLower(mode);
	
	if (fixedMode == "cull")
		ret = TGen::PolygonFaceCull;
	else if (fixedMode == "lines" || fixedMode == "line")
		ret = TGen::PolygonFaceLines;
	else if (fixedMode == "point" || fixedMode == "points")
		ret = TGen::PolygonFacePoints;
	else if (fixedMode == "fill")
		ret = TGen::PolygonFaceFill;
	else
		throw TGen::RuntimeException("StringToPolygonFaceMode", "invalid back mode: '" + mode + "'!");	
	
	return ret;
}
