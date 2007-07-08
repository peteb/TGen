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

TGen::RenderContext::RenderContext() : depthWrite(true), frontColor(TGen::Color::White), shader(NULL), depthFunc(TGen::CompareLess),
	front(TGen::PolygonFaceFill), back(TGen::PolygonFaceCull), blendSrc(TGen::BlendSourceAlpha), blendDst(TGen::BlendOneMinusSourceAlpha) 
{

}

void TGen::RenderContext::AddTextureUnit(TGen::TextureUnit * unit) {
	textureUnits.push_back(unit);
}

TGen::TextureUnit::TextureUnit(int unit, TGen::Texture * texture) 
	: unit(unit), texture(texture), 
	genU(TGen::TextureCoordGenBase), genV(TGen::TextureCoordGenBase) {}

