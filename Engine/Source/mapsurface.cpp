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
	: TGen::NewMesh(material, "")
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

// TODO: bind vertex var to shader somehow

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

TGen::NewMeshInstance * TGen::Engine::MapSurface::instantiate(TGen::VertexDataSource & source) {
	if (vb || ib)
		throw TGen::RuntimeException("MapSurface::createVertexData", "vertex data already created!");
	
	vb = source.createVertexData(VertexDecl(), vertices.size() * sizeof(VertexDecl::Type), TGen::UsageStatic);
	ib = source.createVertexData(IndexDecl(), indices.size() * sizeof(IndexDecl::Type), TGen::UsageStatic);
	
	// offset indices, don't do this twice on the same index set!!
	for (int i = 0; i < indices.size(); ++i)
		indices[i] += vb->getReadOffset();
	
	vb->bufferData(&vertices[0], vertices.size() * sizeof(VertexDecl::Type), 0);
	ib->bufferData(&indices[0], indices.size() * sizeof(IndexDecl::Type), 0);
	
	return this;
}

void TGen::Engine::MapSurface::updateShaderVariable(TGen::ShaderVariable & var, const std::string & name) {
	//std::cout << "WANT TO BIND " << name << std::endl;
	//if (name == "vertexTangent")
		vb->bindShaderVariable(10, var);
	//var = TGen::Vector4(0.0, 0.0, 1.0, 1.0);
}

void TGen::Engine::MapSurface::calculateTangents() {
	TGen::Vector3 * tan1 = new TGen::Vector3[vertices.size() * 2];
	TGen::Vector3 * tan2 = tan1 + vertices.size();
	
	for (uint i = 0; i < indices.size(); i += 3) {
		uint i1 = indices[i + 0];
		uint i2 = indices[i + 1];
		uint i3 = indices[i + 2];
		
		// TODO: fix this...
		TGen::Vector3 v1 = TGen::Vector3(vertices[i1].Vertex::Type::x, vertices[i1].Vertex::Type::y, vertices[i1].Vertex::Type::z);
		TGen::Vector3 v2 = TGen::Vector3(vertices[i2].Vertex::Type::x, vertices[i2].Vertex::Type::y, vertices[i2].Vertex::Type::z);
		TGen::Vector3 v3 = TGen::Vector3(vertices[i3].Vertex::Type::x, vertices[i3].Vertex::Type::y, vertices[i3].Vertex::Type::z);
		
		TGen::Vector2 w1 = TGen::Vector2(vertices[i1].u, vertices[i1].v);
		TGen::Vector2 w2 = TGen::Vector2(vertices[i2].u, vertices[i2].v);
		TGen::Vector2 w3 = TGen::Vector2(vertices[i3].u, vertices[i3].v);
		
		float x1 = v2.x - v1.x;
		float x2 = v3.x - v1.x;
		float y1 = v2.y - v1.y;
		float y2 = v3.y - v1.y;
		float z1 = v2.z - v1.z;
		float z2 = v3.z - v1.z;
		
		float s1 = w2.x - w1.x;
		float s2 = w3.x - w1.x;
		float t1 = w2.y - w1.y;
		float t2 = w3.y - w1.y;
		
		float r = 1.0f / (s1 * t2 - s2 * t1);
		TGen::Vector3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
		TGen::Vector3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r);
		
		tan1[i1] += sdir;
		tan1[i2] += sdir;
		tan1[i3] += sdir;
		
		tan2[i1] += tdir;
		tan2[i2] += tdir;
		tan2[i3] += tdir;
	}
	
	for (uint a = 0; a < vertices.size(); ++a) {
		TGen::Vector3 n = TGen::Vector3(vertices[a].Normal::Type::nx, vertices[a].Normal::Type::ny, vertices[a].Normal::Type::nz);
		const TGen::Vector3 & t = tan1[a];
		
		// Gram-Schmidt orthogonalize
		TGen::Vector4 tangent = TGen::Vector4((t - n * TGen::Vector3::DotProduct(n, t)).normalize());
		tangent.w = (TGen::Vector3::DotProduct(TGen::Vector3::CrossProduct(n, t), tan2[a]) < 0.0f) ? -1.0f : 1.0f;
		
		vertices[a].Tangent::Type::x = tangent.x;
		vertices[a].Tangent::Type::y = tangent.y;
		vertices[a].Tangent::Type::z = tangent.z;
		vertices[a].Tangent::Type::w = tangent.w;
		
		// Calculate handedness
		
	}
	
	delete[] tan1;
}

void TGen::Engine::MapSurface::swapWinding() {
	for (int i = 0; i < indices.size(); i += 3) {
		std::swap(indices[i + 0], indices[i + 2]);
	}
	
}

