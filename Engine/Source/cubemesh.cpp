/*
 *  cubemesh.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/26/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "cubemesh.h"

TGen::Engine::CubeMesh::CubeMesh(const std::string & materialName, const std::string & materialNamePostfix, const TGen::Vector3 & min, const TGen::Vector3 & max, const TGen::Vector3 & texMin, const TGen::Vector3 & texMax) 
	: TGen::NewMesh(materialName, materialNamePostfix)
	, vb(NULL)
	, min(min)
	, max(max)
	, texMin(texMin)
	, texMax(texMax)
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
	float vertices[24 * 12] = 
		{
			// FRONT
			max.x, min.y, max.z,  0.0f, 0.0f, 1.0f,  texMax.x, texMin.y,  1.0f, 0.0f, 0.0f,			-1.0f, 
			min.x, min.y, max.z,  0.0f, 0.0f, 1.0f,  texMin.x, texMin.y,  1.0f, 0.0f, 0.0f,			-1.0f, 
			min.x, max.y, max.z,  0.0f, 0.0f, 1.0f,  texMin.x, texMax.y,  1.0f, 0.0f, 0.0f, 		-1.0f, 
			max.x, max.y, max.z,  0.0f, 0.0f, 1.0f,  texMax.x, texMax.y,  1.0f, 0.0f, 0.0f, 		-1.0f, 
			
			// RIGHT
			
			max.x, min.y, min.z,  1.0f, 0.0f, 0.0f,  texMax.z, texMin.y,  0.0f, 0.0f, -1.0f, 		-1.0f, 
			max.x, min.y, max.z,  1.0f, 0.0f, 0.0f,  texMin.z, texMin.y,  0.0f, 0.0f, -1.0f, 		-1.0f, 
			max.x, max.y, max.z,  1.0f, 0.0f, 0.0f,  texMin.z, texMax.y,  0.0f, 0.0f, -1.0f, 		-1.0f, 
			max.x, max.y, min.z,  1.0f, 0.0f, 0.0f,  texMax.z, texMax.y,  0.0f, 0.0f, -1.0f, 		-1.0f, 

			// BACK
			
			min.x, min.y, min.z,  0.0f, 0.0f, -1.0f,  texMax.x, texMin.y,  -1.0f, 0.0f, 0.0f, 		-1.0f, 
			max.x, min.y, min.z,  0.0f, 0.0f, -1.0f,  texMin.x, texMin.y,  -1.0f, 0.0f, 0.0f, 		-1.0f, 
			max.x, max.y, min.z,  0.0f, 0.0f, -1.0f,  texMin.x, texMax.y,  -1.0f, 0.0f, 0.0f, 		-1.0f, 
			min.x, max.y, min.z,  0.0f, 0.0f, -1.0f,  texMax.x, texMax.y,  -1.0f, 0.0f, 0.0f, 		-1.0f, 
			
			// LEFT

			min.x, min.y, max.z,  -1.0f, 0.0f, 0.0f,  texMax.z, texMin.y,  0.0f, 0.0f, 1.0f, 		-1.0f, 
			min.x, min.y, min.z,  -1.0f, 0.0f, 0.0f,  texMin.z, texMin.y,  0.0f, 0.0f, 1.0f, 		-1.0f, 
			min.x, max.y, min.z,  -1.0f, 0.0f, 0.0f,  texMin.z, texMax.y,  0.0f, 0.0f, 1.0f, 		-1.0f, 
			min.x, max.y, max.z,  -1.0f, 0.0f, 0.0f,  texMax.z, texMax.y,  0.0f, 0.0f, 1.0f, 		-1.0f, 

			// BOTTOM
			max.x, min.y, min.z,  0.0f, -1.0f, 0.0f,  texMax.x, texMin.z,  1.0f, 0.0f, 0.0f, 		-1.0f, 
			min.x, min.y, min.z,  0.0f, -1.0f, 0.0f,  texMin.x, texMin.z,  1.0f, 0.0f, 0.0f, 		-1.0f, 
			min.x, min.y, max.z,  0.0f, -1.0f, 0.0f,  texMin.x, texMax.z,  1.0f, 0.0f, 0.0f, 		-1.0f, 
			max.x, min.y, max.z,  0.0f, -1.0f, 0.0f,  texMax.x, texMax.z,  1.0f, 0.0f, 0.0f, 		-1.0f, 
			
			// TOP
			max.x, max.y, max.z,  0.0f, 1.0f, 0.0f,  texMax.x, texMax.z,  -1.0f, 0.0f, 0.0f, 		1.0f, 
			min.x, max.y, max.z,  0.0f, 1.0f, 0.0f,  texMin.x, texMax.z,  -1.0f, 0.0f, 0.0f, 		1.0f, 
			min.x, max.y, min.z,  0.0f, 1.0f, 0.0f,  texMin.x, texMin.z,  -1.0f, 0.0f, 0.0f, 		1.0f, 
			max.x, max.y, min.z,  0.0f, 1.0f, 0.0f,  texMax.x, texMin.z,  -1.0f, 0.0f, 0.0f, 		1.0f, 
			
		};
		
		
	// TODO: getVariable variable 'r_syncVtrace' does not exist     när inte vardump finns
	
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

void TGen::Engine::CubeMesh::updateShaderVariable(TGen::ShaderVariable & var, const std::string & name) {
	//std::cout << "Hey" << std::endl;
	vb->bindShaderVariable(10, var);
}

TGen::Engine::CubeMesh * TGen::Engine::CubeMesh::instantiate(TGen::VertexDataSource & source) {
	throw TGen::RuntimeException("CubeMesh::instantiate", "trying to instantiate static mesh!");
}
