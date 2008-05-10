/*
 *  md3animmeshdouble.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/7/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "md3animmeshdouble.h"
#include "md3animmesh.h"
#include <tgen_graphics.h>

TGen::MD3::AnimatingMeshDouble::AnimatingMeshDouble(const std::string & materialName, TGen::MD3::AnimatingMesh const & base) 
	: TGen::MD3::AnimatingMeshInstance(materialName, "gpuinterpol", base)
	, ib(NULL)
	, vb(NULL)
	, startIndex(0)
	, numIndices(0)
	, numVertices(0)
	, doubleLastStart(-1)
	, doubleLastEnd(-1)
	, dtVar(NULL)
{
	
}

TGen::MD3::AnimatingMeshDouble::~AnimatingMeshDouble() {
	//delete ib;		// TODO: PURE VIRTUAL METHOD CALLED; WTF??!
	//delete vb;
}

void TGen::MD3::AnimatingMeshDouble::preRender(TGen::Renderer & renderer) const {
	renderer.setVertexBuffer(vb);
	renderer.setIndexBuffer(ib);	
}

void TGen::MD3::AnimatingMeshDouble::render(TGen::Renderer & renderer) const {
	renderer.drawIndexedPrimitive(TGen::PrimitiveTriangles, startIndex, numIndices);		
}

void TGen::MD3::AnimatingMeshDouble::createVertexData(TGen::VertexDataSource & dataSource) {
	numIndices = base.getNumIndices();
	numVertices = base.getNumVertices();
	
	ib = dataSource.createVertexData(TGen::MD3::IndexDecl(), sizeof(TGen::MD3::IndexDecl::Type) * numIndices, TGen::UsageStatic);
	vb = dataSource.createVertexData(TGen::MD3::DoubleVertexDecl(), sizeof(TGen::MD3::DoubleVertexDecl::Type) * numVertices, TGen::UsageStream);			
	

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

void TGen::MD3::AnimatingMeshDouble::updateVertices(int frameNum, scalar t) {
	dt = t;
	
	if (dtVar)
		*dtVar = float(dt); //TGen::Clamp(dt, 0.0f, 1.0f);
	
	updateDoubleVertices(frameNum, frameNum + 1);
}

void TGen::MD3::AnimatingMeshDouble::updateDoubleVertices(int start, int end) {
	if (start >= base.getNumAnimationFrames())
		start = 0;
	if (end >= base.getNumAnimationFrames())
		end = 0;
	
	if (start == doubleLastStart && end == doubleLastEnd)
		return;

	doubleLastStart = start;
	doubleLastEnd = end;
	
	//std::cout << "Update double vertices: " << start << ":" << end << std::endl;
	
	std::vector<TGen::MD3::DoubleVertexDecl::Type> vertices;		// TODO: cache
	vertices.reserve(base.getNumVertices());
	
	
	TGen::MD3::AnimationFrame const & frame1 = base.getAnimationFrame(start);
	TGen::MD3::AnimationFrame const & frame2 = base.getAnimationFrame(end);
	
	// TODO: optimering: hälften så mycket bandbredd för överföring till grafikkort:
	//			ha en egen vb för "start" och "end", uppdatera bara varannan gång men
	//       ändra riktning på dt som skickas till shader
	//	      (kräver stöd för detta i TGen Graphics/OGL)
	
	for (int i = 0; i < frame1.vertices.size(); ++i) {
		TGen::MD3::DoubleVertexDecl::Type vertex;
		
		vertex.u = base.getTexcoord(i).u;
		vertex.v = base.getTexcoord(i).v;
		
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

void TGen::MD3::AnimatingMeshDouble::updateShaderVariable(TGen::ShaderVariable & var, const std::string & name) {
	if (name == "nextFrameVert")
		vb->bindShaderVariable(40, var);
	else if (name == "nextFrameNormal")
		vb->bindShaderVariable(41, var);		
	else if (name == "frameTime") {
		dtVar = &var;
	}

}
