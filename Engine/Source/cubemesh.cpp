/*
 *  cubemesh.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/26/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "cubemesh.h"

TGen::Engine::CubeMesh::CubeMesh(const std::string & materialName, const std::string & materialNamePostfix, const TGen::Vector3 & min, const TGen::Vector3 & max) 
	: TGen::NewMesh(materialName, materialNamePostfix)
	, vb(NULL)
	, min(min)
	, max(max)
{
	
}

TGen::Engine::CubeMesh::~CubeMesh() {
	delete vb;
}

void TGen::Engine::CubeMesh::preRender(TGen::Renderer & renderer) const {
	renderer.setVertexBuffer(vb);
}

void TGen::Engine::CubeMesh::render(TGen::Renderer & renderer) const {
	renderer.drawPrimitive(TGen::PrimitiveQuads, 0, 4 * 6);
}

void TGen::Engine::CubeMesh::createVertexData(TGen::VertexDataSource & dataSource) {
	std::vector<DoubleVertexDecl::Type> vertices;
	
	// fräs in
	
	vb = dataSource.createVertexData(DoubleVertexDecl(), sizeof(DoubleVertexDecl::Type) * vertices.size(), TGen::UsageStatic);
	vb->bufferData(&vertices[0], sizeof(DoubleVertexDecl::Type) * vertices.size(), 0);
}

TGen::Engine::CubeMesh * TGen::Engine::CubeMesh::instantiate() const {
	throw TGen::RuntimeException("CubeMesh::instantiate", "trying to instantiate static mesh!");
}
