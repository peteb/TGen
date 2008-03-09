/*
 *  md5meshinst.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/10/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "md5meshinst.h"

TGen::MD5::MeshInstance::MeshInstance(const std::string & materialName, 
												  const std::string & materialNamePostfix, 
												  TGen::MD5::Mesh const & base) 
	: TGen::NewMeshInstance(materialName, materialNamePostfix)
	, base(base)
	, ib(NULL)
	, vb(NULL)
	, startIndex(0)
	, numIndices(0)
	, numVertices(0)
{
	
}


TGen::MD5::MeshInstance::~MeshInstance() {
	//delete ib;
	//delete vb;
}

void TGen::MD5::MeshInstance::preRender(TGen::Renderer & renderer) const {
	//renderer.setVertexBuffer(vb);
	//renderer.setIndexBuffer(ib);
}

void TGen::MD5::MeshInstance::render(TGen::Renderer & renderer) const {
	renderer.drawIndexedPrimitive(TGen::PrimitiveTriangles, startIndex, numIndices);		
}

void TGen::MD5::MeshInstance::updateVertices(int frame, scalar t) {
	
}

void TGen::MD5::MeshInstance::updateInterpolatedVertices(int frame1, int frame2, scalar t) {
	
}

void TGen::MD5::MeshInstance::createVertexData(TGen::VertexDataSource & dataSource) {
	// create buffers according to base (that's where the vertex data is stored)
}

