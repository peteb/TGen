/*
 *  vbrenderable.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 11/28/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "vbrenderable.h"

TGen::Engine::VBRenderable::VBRenderable(TGen::VertexData * vb, TGen::PrimitiveType type, uint size)
	: vb(vb)
	, size(size)
	, type(type)
{
	
}

void TGen::Engine::VBRenderable::preRender(TGen::Renderer & renderer) const {
	renderer.setVertexBuffer(vb);
}

void TGen::Engine::VBRenderable::render(TGen::Renderer & renderer) const {
	renderer.drawPrimitive(type, 0, size);
}

