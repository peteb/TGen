/*
 *  cube.cpp
 *  The Quaker
 *
 *  Created by Peter Backman on 7/20/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "cube.h"

using namespace TGen;

Cube::Cube(TGen::Renderer & renderer)
	: vb(NULL)
	, ib(NULL)
	, width(1.0f)
	, height(1.0f)
	, depth(0.3f)
{
	
	scalar halfWidth = width / 2.0f, halfHeight = height / 2.0f, halfDepth = depth / 2.0f;
		
	vb = renderer.createVertexBuffer(MyVertex(), sizeof(MyVertex::Type) * 8, TGen::UsageStatic);
	ib = renderer.createIndexBuffer(MyIndex(), sizeof(MyIndex::Type) * 24, TGen::UsageStatic);
	
	MyVertex::Type vertices[8] = {
		MyVertex::Type(Vector3(-halfWidth, halfHeight, halfDepth), Vector2(0.0f, 0.0f)),		// 0
		MyVertex::Type(Vector3(halfWidth, halfHeight, halfDepth), Vector2(1.0f, 0.0f)),			// 1
		MyVertex::Type(Vector3(halfWidth, -halfHeight, halfDepth), Vector2(1.0f, 1.0f)),		// 2
		MyVertex::Type(Vector3(-halfWidth, -halfHeight, halfDepth), Vector2(0.0f, 1.0f)),		// 3

		MyVertex::Type(Vector3(-halfWidth, halfHeight, -halfDepth), Vector2(0.0f, 1.0f)),		// 4
		MyVertex::Type(Vector3(halfWidth, halfHeight, -halfDepth), Vector2(1.0f, 1.0f)),		// 5
		MyVertex::Type(Vector3(halfWidth, -halfHeight, -halfDepth), Vector2(1.0f, 0.0f)),		// 6
		MyVertex::Type(Vector3(-halfWidth, -halfHeight, -halfDepth), Vector2(0.0f, 0.0f)),		// 7
	};

	MyIndex::Type indicies[24] = {
		0, 1, 2, 3,		// FRONT
		7, 6, 5, 4,		// BACK
		1, 5, 6, 2,		// RIGHT
		4, 0, 3, 7,		// LEFT
		0, 4, 5, 1,		// TOP
		2, 6, 7, 3,		// BOTTOM
	};

	vb->bufferData(vertices, sizeof(MyVertex::Type) * 8, 0);
	ib->bufferData(indicies, sizeof(MyIndex::Type) * 24, 0);	
}

Cube::~Cube() {
	delete vb;
	delete ib;
}


void Cube::prepareRender(TGen::Renderer & renderer) const {
	renderer.setVertexBuffer(vb);
	renderer.setIndexBuffer(ib);			
}

void Cube::render(TGen::Renderer & renderer) const {
	renderer.drawIndexedPrimitive(TGen::PrimitiveQuads, 0, 24);
}

TGen::Vector3 Cube::getMax() const {
	return TGen::Vector3(width / 2.0f, height / 2.0f, depth / 2.0f);
}

TGen::Vector3 Cube::getMin() const {
	return TGen::Vector3(-width / 2.0f, -height / 2.0f, -depth / 2.0f);
}

