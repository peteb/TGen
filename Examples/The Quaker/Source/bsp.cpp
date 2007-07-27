/*
 *  bsp.cpp
 *  The Quaker
 *
 *  Created by Peter Backman on 7/21/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "bsp.h"
#include <tgen_graphics.h>

BSPTree::BSPTree(const std::string & name)
	: SceneNode(name)
	, vb(NULL)
{
		
}

BSPTree::~BSPTree() {
	delete vb;
}

BSPGeometry::BSPGeometry(BSPTree & tree, bool wire)
	: tree(tree)
	, startIndex(0)
	, numIndices(0)
	, ib(NULL)
	, vb(NULL)
	, wire(wire)
{
	
}

BSPGeometry::~BSPGeometry() {
	//delete ib;
	//delete vb;
}


void BSPGeometry::PrepareRender(TGen::Renderer & renderer) const {
	if (!vb)
		renderer.setVertexBuffer(tree.vb);
	else
		renderer.setVertexBuffer(vb);
	
	if (!ib)
		renderer.setIndexBuffer(tree.ib);		
	else
		renderer.setIndexBuffer(ib);

}

//#include <GLUT/GLUT.h>

void BSPGeometry::Render(TGen::Renderer & renderer) const {
	//if (polygon)
	//	renderer.DrawIndexedPrimitive(TGen::PrimitiveTriangleFan, startIndex, numIndices);
	//else
	//glCullFace(GL_FRONT);
	//if (polygon)
	
	TGen::PrimitiveType type = TGen::PrimitiveTriangles;
	if (wire) {
		type = TGen::PrimitiveTriangleStrip;
	}

	
	if (multidraw.empty()) {
		if (ib || tree.ib)
			renderer.DrawIndexedPrimitive(type, startIndex, numIndices);
		else
			renderer.DrawPrimitive(type, startIndex, numIndices);
	}
	else {
		for (int i = 0; i < multidraw.size(); ++i) {
			int count = multidraw[i].second;
			
			renderer.setIndexBuffer(multidraw[i].first);
			renderer.DrawIndexedPrimitive(type, 0, count);
		}
	}
	
}

TGen::Vector3 BSPGeometry::getMax() const {
	return TGen::Vector3(1.0f, 1.0f, 1.0f);	
}

TGen::Vector3 BSPGeometry::getMin() const {
	return TGen::Vector3(-1.0f, -1.0f, -1.0f);
}

