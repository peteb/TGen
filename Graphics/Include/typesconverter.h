/*
 *  typesconverter.h
 *  TGen Graphics
 *
 *  Created by Peter Backman on 1/7/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_GRAPHICS_TYPESCONVERTER_H
#define _TGEN_GRAPHICS_TYPESCONVERTER_H

#include "renderer_types.h"

namespace TGen {
	TGen::BlendFunc StringToBlendFunc(const std::string & blend);
	TGen::CompareFunc StringToCompareFunc(const std::string & func);
	TGen::PolygonFaceMode StringToPolygonFaceMode(const std::string & mode);
	TGen::TextureFilter StringToTextureFilter(const std::string & filter);
	
} // !TGen

#endif // !_TGEN_GRAPHICS_TYPESCONVERTER_H
