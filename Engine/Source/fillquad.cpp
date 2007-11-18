/*
 *  fillquad.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 11/11/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "fillquad.h"
#include <iostream>

TGen::Engine::FillQuad::FillQuad(const TGen::Vector2 & min, const TGen::Vector2 & max, TGen::Renderer & renderer) 
	: TGen::Mesh("fillquad")
	, min(min)
	, max(max)
	, vb(NULL)
{
	VertexDecl::Type vertices[4] = {		// TODO: varför måste det här vara cw när ccw körs globalt?
		VertexDecl::Type(TGen::Vector2(min.x, min.y), TGen::Vector2(0.0f, 0.0f)),
		VertexDecl::Type(TGen::Vector2(max.x, min.y), TGen::Vector2(1.0f, 0.0f)),
		VertexDecl::Type(TGen::Vector2(max.x, max.y), TGen::Vector2(1.0f, 1.0f)),
		VertexDecl::Type(TGen::Vector2(min.x, max.y), TGen::Vector2(0.0f, 1.0f)),
	};
	
	vb = renderer.createVertexBuffer(VertexDecl(), sizeof(VertexDecl::Type) * 4, TGen::UsageStatic);
	vb->bufferData(vertices, sizeof(VertexDecl::Type) * 4, 0);
}

TGen::Engine::FillQuad::~FillQuad() {
	delete vb;
}

void TGen::Engine::FillQuad::preRender(TGen::Renderer & renderer) const {
	renderer.setVertexBuffer(vb);
}

void TGen::Engine::FillQuad::render(TGen::Renderer & renderer) const {
	renderer.drawPrimitive(TGen::PrimitiveQuads, 0, 4);
}

TGen::Vector3 TGen::Engine::FillQuad::getMax() const {
	return max;
}

TGen::Vector3 TGen::Engine::FillQuad::getMin() const {
	return min;
}

TGen::Vector3 TGen::Engine::FillQuad::getOrigin() const {
	return (min + max) / 2.0f;
}
