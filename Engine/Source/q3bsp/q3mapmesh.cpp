/*
 *  q3mapmesh.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/27/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include "q3mapmesh.h"

TGen::Engine::Q3MapMesh::Q3MapMesh(const std::string & material)
	: TGen::NewMesh(material, "")
	, vb(NULL)
{
}


TGen::NewMeshInstance * TGen::Engine::Q3MapMesh::instantiate(TGen::VertexDataSource & source) {
	if (vb)
		throw TGen::RuntimeException("Q3MapMesh::instantiate", "vertex data already created!");
	
	vb = source.createVertexData(VertexDecl(), vertices.size() * sizeof(VertexDecl::Type), TGen::UsageStatic);
	vb->bufferData(&vertices[0], vertices.size() * sizeof(VertexDecl::Type), 0);

	return this;	
}

void TGen::Engine::Q3MapMesh::addVertex(const VertexDecl::Type & vertex) {
	vertices.push_back(vertex);
}

void TGen::Engine::Q3MapMesh::preRender(TGen::Renderer & renderer) const {
	if (!vb)
		throw TGen::RuntimeException("MapSurface::preRender", "no vertexdata loaded for map surface!");
	
	renderer.setVertexBuffer(vb);
}

// TODO: bind vertex var to shader somehow

void TGen::Engine::Q3MapMesh::render(TGen::Renderer & renderer) const {
	renderer.drawPrimitive(TGen::PrimitiveTriangles, 0, vertices.size());
}