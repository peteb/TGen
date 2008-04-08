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
	renderer.drawPrimitive(TGen::PrimitiveQuads, 0, 24); //4 * 6);
}

void TGen::Engine::CubeMesh::createVertexData(TGen::VertexDataSource & dataSource) {
	float vertices[24 * 8] = 
		{
			// FRONT
			min.x, min.y, max.z, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			max.x, min.y, max.z, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
			max.x, max.y, max.z, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
			min.x, max.y, max.z, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			
			// RIGHT
			max.x, min.y, max.z, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			max.x, min.y, min.z, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			max.x, max.y, min.z, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			max.x, max.y, max.z, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			
			// BACK
			max.x, min.y, min.z, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
			min.x, min.y, min.z, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
			min.x, max.y, min.z, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
			max.x, max.y, min.z, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
			
			// LEFT
			min.x, min.y, min.z, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			min.x, min.y, max.z, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			min.x, max.y, max.z, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			min.x, max.y, min.z, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			
			// TOP
			min.x, min.y, min.z, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			max.x, min.y, min.z, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			max.x, min.y, max.z, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
			min.x, min.y, max.z, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
			
			// BOTTOM
			min.x, max.y, max.z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			max.x, max.y, max.z, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			max.x, max.y, min.z, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			min.x, max.y, min.z, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,

		};
		
		
		
		// fräs in
	
	/*	typedef TGen::TexCoord2<float, 0> TexCoordDecl;
		typedef TGen::Vertex3<float> VertexCoordDecl;
		typedef TGen::Normal3<float> NormalDecl;
		typedef TGen::VertexAttribute3<40, float> SecondVertexCoordDecl;
		typedef TGen::VertexAttribute3<41, float> SecondNormalDecl;
		
		typedef TGen::JoinVertexElements3<VertexCoordDecl, NormalDecl, TexCoordDecl>			VertexDecl;
	*/
		
	vb = dataSource.createVertexData(VertexDecl(), sizeof(VertexDecl::Type) * 24, TGen::UsageStatic);
	vb->bufferData(vertices, sizeof(VertexDecl::Type) * 24, 0);
}

TGen::Engine::CubeMesh * TGen::Engine::CubeMesh::instantiate() const {
	throw TGen::RuntimeException("CubeMesh::instantiate", "trying to instantiate static mesh!");
}
