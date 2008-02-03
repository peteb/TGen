/*
 *  md3submesh.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/26/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "md3submesh.h"
#include <tgen_graphics.h>

TGen::MD3::Submesh::Submesh(const std::string & materialName) 
	: TGen::NewMesh(materialName)
	, vb(NULL)
	, ib(NULL)
	, primitive(TGen::PrimitiveTriangles)
	, startIndex(0)
	, indexCount(0)
{
}

TGen::MD3::Submesh::~Submesh() {
	delete vb;
	delete ib;
}

void TGen::MD3::Submesh::preRender(TGen::Renderer & renderer) const {
	renderer.setVertexBuffer(vb);
	renderer.setIndexBuffer(ib);
}

void TGen::MD3::Submesh::render(TGen::Renderer & renderer) const {
	renderer.drawIndexedPrimitive(primitive, startIndex, indexCount);	
}

TGen::MD3::Submesh * TGen::MD3::Submesh::instantiate() const {
	throw TGen::RuntimeException("MD3::Mesh::instantiate", "trying to instantiate static mesh!");
}
