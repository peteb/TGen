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

TGen::RenderContext::RenderContext() 
	: depthWrite(true)
	, frontColor(TGen::Color::White)
	, shader(NULL)
	, depthFunc(TGen::CompareLess)
	, front(TGen::PolygonFaceFill)
	, back(TGen::PolygonFaceCull)
	, blendSrc(TGen::BlendSourceAlpha)
	, blendDst(TGen::BlendOneMinusSourceAlpha)
	, colorFromVertex(false)
	, disableDepth(false)
{

}

void TGen::RenderContext::addTextureUnit(TGen::TextureUnit * unit) {
	textureUnits.push_back(unit);
}

