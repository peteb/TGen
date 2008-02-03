/*
 *  md3animmeshinst.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/3/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "md3animmeshinst.h"
#include "md3animmesh.h"
#include <tgen_graphics.h>

TGen::MD3::AnimatingMeshInstance::AnimatingMeshInstance(const std::string & materialName, TGen::MD3::AnimatingMesh & base) 
	: TGen::NewMeshInstance(materialName)
	, base(base)
	, ib(NULL)
	, vb(NULL)
	, primitive(TGen::PrimitiveTriangles)
	, startIndex(0)
	, indexCount(0)
{
	
}

TGen::MD3::AnimatingMeshInstance::~AnimatingMeshInstance() {
	//delete ib;		// TODO: PURE VIRTUAL METHOD CALLED; WTF??!
	//delete vb;
}

void TGen::MD3::AnimatingMeshInstance::preRender(TGen::Renderer & renderer) const {
	renderer.setVertexBuffer(vb);
	renderer.setIndexBuffer(ib);	
}

void TGen::MD3::AnimatingMeshInstance::render(TGen::Renderer & renderer) const {
	renderer.drawIndexedPrimitive(primitive, startIndex, indexCount);		
}

void TGen::MD3::AnimatingMeshInstance::updateVertices(int frameNum) {
	vertices.reserve(base.vertexCount);
	vertices.clear();
	
	if (frameNum >= base.getNumAnimationFrames())
		frameNum = 0;
	
	TGen::MD3::AnimationFrame const & frame = base.getAnimationFrame(frameNum);
	
	for (int i = 0; i < frame.vertices.size(); ++i) {
		TGen::MD3::VertexDecl::Type vertex;
		vertex.x = frame.vertices[i].x;
		vertex.y = frame.vertices[i].y;
		vertex.z = frame.vertices[i].z;
		vertex.nx = frame.vertices[i].nx;
		vertex.ny = frame.vertices[i].ny;
		vertex.nz = frame.vertices[i].nz;
		vertex.u = base.texcoords[i].u;
		vertex.v = base.texcoords[i].v;
		
		vertices.push_back(vertex);
	}
	
	vb->bufferData(&vertices[0], sizeof(TGen::MD3::VertexDecl::Type) * vertices.size(), 0);
}
