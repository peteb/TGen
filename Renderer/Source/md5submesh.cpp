/*
 *  md5submesh.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 1/23/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "md5submesh.h"

TGen::MD5::Submesh::Submesh() 
	: vb(NULL)
	, ib(NULL)
	, primitive(TGen::PrimitiveTriangles)
	, startIndex(0)
	, indexCount(0)
{
	
}

TGen::MD5::Submesh::~Submesh() {
	delete vb;
	delete ib;	
}

void TGen::MD5::Submesh::preRender(TGen::Renderer & renderer) const {
	renderer.setVertexBuffer(vb);
	renderer.setIndexBuffer(ib);
}

void TGen::MD5::Submesh::render(TGen::Renderer & renderer) const {
	renderer.drawIndexedPrimitive(primitive, startIndex, indexCount);	
}
