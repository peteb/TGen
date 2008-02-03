/*
 *  md3mesh.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/26/07.
 *  Renamed by Peter Backman on 2/3/08.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "md3mesh.h"
#include <tgen_graphics.h>

TGen::MD3::Mesh::Mesh(const std::string & materialName) 
	: TGen::NewMesh(materialName)
	, vb(NULL)
	, ib(NULL)
	, primitive(TGen::PrimitiveTriangles)
	, startIndex(0)
	, indexCount(0)
{
}

TGen::MD3::Mesh::~Mesh() {
	delete vb;
	delete ib;
}

void TGen::MD3::Mesh::preRender(TGen::Renderer & renderer) const {
	renderer.setVertexBuffer(vb);
	renderer.setIndexBuffer(ib);
}

void TGen::MD3::Mesh::render(TGen::Renderer & renderer) const {
	renderer.drawIndexedPrimitive(primitive, startIndex, indexCount);	
}

TGen::MD3::Mesh * TGen::MD3::Mesh::instantiate() const {
	throw TGen::RuntimeException("MD3::Mesh::instantiate", "trying to instantiate static mesh!");
}
