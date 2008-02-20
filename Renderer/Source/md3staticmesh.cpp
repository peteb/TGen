/*
 *  md3staticmesh.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/26/07.
 *  Renamed by Peter Backman on 2/3/08.
 *  Renamed by Peter Backman on 2/7/08.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "md3staticmesh.h"
#include <tgen_graphics.h>

TGen::MD3::StaticMesh::StaticMesh(const std::string & materialName, const std::string & materialNamePostfix) 
	: TGen::NewMesh(materialName, materialNamePostfix)
	, vb(NULL)
	, ib(NULL)
	, startIndex(0)
	, numIndices(0)
{
}

TGen::MD3::StaticMesh::~StaticMesh() {
	delete vb;
	delete ib;
}

void TGen::MD3::StaticMesh::preRender(TGen::Renderer & renderer) const {
	renderer.setVertexBuffer(vb);
	renderer.setIndexBuffer(ib);
}

void TGen::MD3::StaticMesh::render(TGen::Renderer & renderer) const {
	renderer.drawIndexedPrimitive(TGen::PrimitiveTriangles, startIndex, numIndices);	
}

TGen::MD3::StaticMesh * TGen::MD3::StaticMesh::instantiate() const {
	throw TGen::RuntimeException("MD3::Mesh::instantiate", "trying to instantiate static mesh! (a *model is stupid)");
}

void TGen::MD3::StaticMesh::createVertexData(TGen::VertexDataSource & dataSource) {
	// används inte, MD3::File::createModel fräser med vb och ib direkt. fult, who cares
}

