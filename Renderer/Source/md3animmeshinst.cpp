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

TGen::MD3::AnimatingMeshInstance::AnimatingMeshInstance(const std::string & materialName, bool doubleVertices, TGen::MD3::AnimatingMesh & base) 
	: TGen::NewMeshInstance(materialName)
	, base(base)
	, ib(NULL)
	, vb(NULL)
	, primitive(TGen::PrimitiveTriangles)
	, startIndex(0)
	, indexCount(0)
	, doubleVertices(doubleVertices)
	, doubleLastStart(-1)
	, doubleLastEnd(-1)
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


// Ska nog inte behövas mer än såhär här, att den spelar rätt frames hör till något ovanför
// det är också upp till lagret ovanför att inte göra onödiga uppdateringar...
void TGen::MD3::AnimatingMeshInstance::updateVertices(int frameNum, scalar t) {
	if (doubleVertices)
		updateDoubleVertices(frameNum, frameNum + 1);
	else
		updateInterpolatedVertices(frameNum, frameNum + 1, t);
}

// AnimatingMeshInstanceSingle, AnimatingMeshInstanceDouble

void TGen::MD3::AnimatingMeshInstance::updateInterpolatedVertices(int start, int end, scalar t) {
	std::vector<TGen::MD3::VertexDecl::Type> vertices;		// TODO: cache
	vertices.reserve(base.vertexCount);
	
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
		
		vertex.u = base.texcoords[i].u;
		vertex.v = base.texcoords[i].v;
		
		vertices.push_back(vertex);
	}
	
	vb->bufferData(&vertices[0], sizeof(TGen::MD3::VertexDecl::Type) * vertices.size(), 0);
}

void TGen::MD3::AnimatingMeshInstance::updateDoubleVertices(int start, int end) {
	if (start >= base.getNumAnimationFrames())
		start = 0;
	if (end >= base.getNumAnimationFrames())
		end = 0;

	if (start == doubleLastStart && end == doubleLastEnd)
		return;
	
	doubleLastStart = start;
	doubleLastEnd = end;
	
	std::cout << "Update double vertices: " << start << ":" << end << std::endl;
	
	std::vector<TGen::MD3::DoubleVertexDecl::Type> vertices;		// TODO: cache
	vertices.reserve(base.vertexCount);
	
	
	TGen::MD3::AnimationFrame const & frame1 = base.getAnimationFrame(start);
	TGen::MD3::AnimationFrame const & frame2 = base.getAnimationFrame(end);
	
	// TODO: optimering: hälften så mycket bandbredd för överföring till grafikkort:
	//			ha en egen vb för "start" och "end", uppdatera bara varannan gång men
	//       ändra riktning på dt som skickas till shader
	//	      (kräver stöd för detta i TGen Graphics/OGL)
	
	for (int i = 0; i < frame1.vertices.size(); ++i) {
		TGen::MD3::DoubleVertexDecl::Type vertex;

		vertex.u = base.texcoords[i].u;
		vertex.v = base.texcoords[i].v;

		vertex.TGen::MD3::VertexCoordDecl::Type::x = frame1.vertices[i].x;
		vertex.TGen::MD3::VertexCoordDecl::Type::y = frame1.vertices[i].y;
		vertex.TGen::MD3::VertexCoordDecl::Type::z = frame1.vertices[i].z;
		vertex.TGen::MD3::NormalDecl::Type::nx = frame1.vertices[i].nx;
		vertex.TGen::MD3::NormalDecl::Type::ny = frame1.vertices[i].ny;
		vertex.TGen::MD3::NormalDecl::Type::nz = frame1.vertices[i].nz;
		
		vertex.TGen::MD3::SecondVertexCoordDecl::Type::x = frame2.vertices[i].x;
		vertex.TGen::MD3::SecondVertexCoordDecl::Type::y = frame2.vertices[i].y;
		vertex.TGen::MD3::SecondVertexCoordDecl::Type::z = frame2.vertices[i].z;
		vertex.TGen::MD3::SecondNormalDecl::Type::x = frame2.vertices[i].nx;
		vertex.TGen::MD3::SecondNormalDecl::Type::y = frame2.vertices[i].ny;
		vertex.TGen::MD3::SecondNormalDecl::Type::z = frame2.vertices[i].nz;
		
		vertices.push_back(vertex);
	}
	
	vb->bufferData(&vertices[0], sizeof(TGen::MD3::DoubleVertexDecl::Type) * vertices.size(), 0);	
}
