/*
 *  mapsurface.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/30/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "mapsurface.h"

TGen::Engine::MapSurface::MapSurface(const std::string & material) 
	: material(material)
	, vb(NULL)
	, ib(NULL)
{
	
}

TGen::Engine::MapSurface::~MapSurface() {
	
}

std::string TGen::Engine::MapSurface::getMaterialName() const {
	return material;
}

void TGen::Engine::MapSurface::addVertex(const VertexDecl::Type & vertex) {
	vertices.push_back(vertex);
}

void TGen::Engine::MapSurface::addIndex(const IndexDecl::Type & index) {
	indices.push_back(index);
}


void TGen::Engine::MapSurface::preRender(TGen::Renderer & renderer) const {
	if (!vb || !ib)
		throw TGen::RuntimeException("MapSurface::preRender", "no vertexdata loaded for map surface!");
	
	renderer.setVertexBuffer(vb);
	renderer.setIndexBuffer(ib);
}

void TGen::Engine::MapSurface::render(TGen::Renderer & renderer) const {
	renderer.drawIndexedPrimitive(TGen::PrimitiveTriangles, 0, indices.size());
}


TGen::Vector3 TGen::Engine::MapSurface::getMax() const {
	return TGen::Vector3::Identity * 1000.0;
}

TGen::Vector3 TGen::Engine::MapSurface::getMin() const {
	return -TGen::Vector3::Identity * 1000.0;	
}

TGen::Vector3 TGen::Engine::MapSurface::getOrigin() const {
	return TGen::Vector3(0.0, 0.0, 0.0);
}

void TGen::Engine::MapSurface::createVertexData(TGen::VertexDataSource & source) {
	if (vb || ib)
		throw TGen::RuntimeException("MapSurface::createVertexData", "vertex data already created!");
	
	vb = source.createVertexData(VertexDecl(), vertices.size() * sizeof(VertexDecl::Type), TGen::UsageStatic);
	ib = source.createVertexData(IndexDecl(), indices.size() * sizeof(IndexDecl::Type), TGen::UsageStatic);
	
	// offset indices, don't do this twice on the same index set!!
	for (int i = 0; i < indices.size(); ++i)
		indices[i] += vb->getReadOffset();
	
	vb->bufferData(&vertices[0], vertices.size() * sizeof(VertexDecl::Type), 0);
	ib->bufferData(&indices[0], indices.size() * sizeof(IndexDecl::Type), 0);
}
