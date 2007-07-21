/*
 *  bsp.cpp
 *  The Quaker
 *
 *  Created by Peter Backman on 7/21/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "bsp.h"

BSPTree::BSPTree(const std::string & name)
	: SceneNode(name)
	, vb(NULL)
{
		
}

BSPTree::~BSPTree() {
	delete vb;
}

BSPGeometry::BSPGeometry(BSPTree & tree, bool polygon)
	: tree(tree)
	, startIndex(0)
	, numIndices(0)
	, ib(NULL)
	, polygon(polygon)
{
	delete ib;
		
}

BSPGeometry::~BSPGeometry() {

}

void BSPGeometry::PrepareRender(TGen::Renderer & renderer) const {
	renderer.setVertexBuffer(tree.vb);
	renderer.setIndexBuffer(ib);

}

//#include <GLUT/GLUT.h>

void BSPGeometry::Render(TGen::Renderer & renderer) const {
	//if (polygon)
	//	renderer.DrawIndexedPrimitive(TGen::PrimitiveTriangleFan, startIndex, numIndices);
	//else
	//glCullFace(GL_FRONT);
	if (polygon)
		renderer.DrawIndexedPrimitive(TGen::PrimitiveTriangles, startIndex, numIndices);
}

TGen::Vector3 BSPGeometry::getMax() const {
	return TGen::Vector3(1.0f, 1.0f, 1.0f);	
}

TGen::Vector3 BSPGeometry::getMin() const {
	return TGen::Vector3(-1.0f, -1.0f, -1.0f);
}

