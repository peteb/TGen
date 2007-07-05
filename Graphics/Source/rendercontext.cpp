/*
 *  rendercontext.cpp
 *  TGen Graphics
 *
 *  Created by Peter Backman on 7/4/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "rendercontext.h"
#include "tgen_core.h"

TGen::RenderContext::RenderContext() : depthWrite(true), frontColor(TGen::Color::White), shader(NULL) {}

void TGen::RenderContext::setTextureUnit(int unit, TGen::Texture * texture) {
	TextureMap::iterator iter = textureUnits.find(unit);
	if (iter != textureUnits.end())
		throw TGen::RuntimeException("RenderContext::setTextureUnit", "texture unit already set");
	
	textureUnits[unit] = texture;
}
