/*
 *  md3animmeshsingle.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/3/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "md3animmeshsingle.h"
#include "md3animmesh.h"
#include <tgen_graphics.h>

TGen::MD3::AnimatingMeshSingle::AnimatingMeshSingle(const std::string & materialName, TGen::MD3::AnimatingMesh const & base) 
	: TGen::MD3::AnimatingMeshInstance(materialName, base)
	, ib(NULL)
	, vb(NULL)
	, startIndex(0)
	, numIndices(0)
	, numVertices(0)
{
	
}

TGen::MD3::AnimatingMeshSingle::~AnimatingMeshSingle() {
	//delete ib;		// TODO: PURE VIRTUAL METHOD CALLED; WTF??!
	//delete vb;
}

void TGen::MD3::AnimatingMeshSingle::preRender(TGen::Renderer & renderer) const {
	renderer.setVertexBuffer(vb);
	renderer.setIndexBuffer(ib);	
}

void TGen::MD3::AnimatingMeshSingle::render(TGen::Renderer & renderer) const {
	renderer.drawIndexedPrimitive(TGen::PrimitiveTriangles, startIndex, numIndices);		
}

void TGen::MD3::AnimatingMeshSingle::createVertexData(TGen::VertexDataSource & dataSource) {
	numVertices = base.getNumVertices();
	numIndices = base.getNumIndices();
	
	ib = dataSource.createVertexData(TGen::MD3::IndexDecl(), sizeof(TGen::MD3::IndexDecl::Type) * numIndices, TGen::UsageStatic);
	vb = dataSource.createVertexData(TGen::MD3::VertexDecl(), sizeof(TGen::MD3::VertexDecl::Type) * numVertices, TGen::UsageStream);
	
	
	TGen::MD3::IndexDecl::Type const * indexArray = base.getIndexArray();
	std::vector<TGen::MD3::IndexDecl::Type> indices;
	indices.reserve(numIndices);
	
	for (int i = 0; i < numIndices; ++i) {
		TGen::MD3::IndexDecl::Type index = indexArray[i];
		index += vb->getReadOffset();
		indices.push_back(index);
	}
	
	ib->bufferData(&indices[0], sizeof(TGen::MD3::IndexDecl::Type) * numIndices, 0);
}

void TGen::MD3::AnimatingMeshSingle::updateVertices(int frameNum, scalar t) {
	updateInterpolatedVertices(frameNum, frameNum + 1, t);
}

void TGen::MD3::AnimatingMeshSingle::updateInterpolatedVertices(int start, int end, scalar t) {
	std::vector<TGen::MD3::VertexDecl::Type> vertices;		// TODO: cache
	vertices.reserve(base.getNumVertices());
	
	if (start >= base.getNumAnimationFrames())
		start = 0;
	if (end >= base.getNumAnimationFrames())
		end = 0;
	
	TGen::MD3::AnimationFrame const & frame1 = base.getAnimationFrame(start);
	TGen::MD3::AnimationFrame const & frame2 = base.getAnimationFrame(end);
	
	for (int i = 0; i < frame1.vertices.size(); ++i) {
		TGen::MD3::VertexDecl::Type vertex;
		vertex.x = TGen::Interpolate(frame1.vertices[i].x, frame2.vertices[i].x, t);
		vertex.y = TGen::Interpolate(frame1.vertices[i].y, frame2.vertices[i].y, t);
		vertex.z = TGen::Interpolate(frame1.vertices[i].z, frame2.vertices[i].z, t);
		vertex.nx = TGen::Interpolate(frame1.vertices[i].nx, frame2.vertices[i].nx, t);
		vertex.ny = TGen::Interpolate(frame1.vertices[i].ny, frame2.vertices[i].ny, t);
		vertex.nz = TGen::Interpolate(frame1.vertices[i].nz, frame2.vertices[i].nz, t);
		
		vertex.u = base.getTexcoord(i).u;
		vertex.v = base.getTexcoord(i).v;
		
		vertices.push_back(vertex);
	}
	
	vb->bufferData(&vertices[0], sizeof(TGen::MD3::VertexDecl::Type) * vertices.size(), 0);
}
