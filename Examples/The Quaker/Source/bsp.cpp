/*
 *  bsp.cpp
 *  The Quaker
 *
 *  Created by Peter Backman on 7/21/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "bsp.h"
#include "camera.h"
#include "renderlist.h"
#include <tgen_graphics.h>

BSPTree::BSPTree(const std::string & name)
	: SceneNode(name)
	, vb(NULL)
	, root(NULL)
{
		
}

BSPTree::~BSPTree() {
	delete vb;
}

BSPGeometry::BSPGeometry(BSPNode & tree, bool wire)
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

BSPNode::BSPNode()
	: leaf1(NULL)
	, leaf2(NULL)
	, front(NULL)
	, back(NULL)
{

}

void BSPGeometry::preRender(TGen::Renderer & renderer) const {
	/*if (!vb)
		renderer.setVertexBuffer(tree.vb);
	else*/
		renderer.setVertexBuffer(vb);
	
/*	if (!ib)
		renderer.setIndexBuffer(tree.ib);		
	else*/
		renderer.setIndexBuffer(ib);

}

#include <GLUT/GLUT.h>

void BSPGeometry::render(TGen::Renderer & renderer) const {
	//if (polygon)
	//	renderer.DrawIndexedPrimitive(TGen::PrimitiveTriangleFan, startIndex, numIndices);
	//else
	//glCullFace(GL_FRONT);
	//if (polygon)
	
	//glDisable(GL_CULL_FACE);
	
	TGen::PrimitiveType type = TGen::PrimitiveTriangles;
	if (wire) {
		type = TGen::PrimitiveTriangleStrip;
	}

	
	if (multidraw.empty()) {
		if (ib)
			renderer.drawIndexedPrimitive(type, startIndex, numIndices);
		else
			renderer.drawPrimitive(type, startIndex, numIndices);
	}
	else {
		for (int i = 0; i < multidraw.size(); ++i) {
			int count = multidraw[i].second;
			
			renderer.setIndexBuffer(multidraw[i].first);
			renderer.drawIndexedPrimitive(type, 0, count);
		}
	}
	
}

TGen::Vector3 BSPGeometry::getMax() const {
	return TGen::Vector3(1.0f, 1.0f, 1.0f);	
}

TGen::Vector3 BSPGeometry::getMin() const {
	return TGen::Vector3(-1.0f, -1.0f, -1.0f);
}

void BSPLeaf::AddToList(RenderList & list) {
	for (int i = 0; i < surfaces.size(); ++i)
		list.AddSurface(&surfaces[i]);
}

void BSPLeaf::AddSurface(const Surface & surf) {
	Surface newSurface = surf;
	//newSurface.Attached(this);
	surfaces.push_back(newSurface);
}

void BSPTree::Accept(SceneNodeVisitor & visitor) {
	visitor.Visit(*this);

	for (int i = 0; i < children.size(); ++i)
		children[i]->Accept(visitor);
}

void BSPTree::getNodes(const Camera & camera, RenderList & list) {
	if (root)
		root->Traverse(camera.getPosition(), list);
}

void BSPNode::Traverse(const TGen::Vector3 & position, RenderList & list, int level) {
	if (!front && !back && !leaf1 && !leaf2)
		return;
	
	/*if (!front && !back) {
		if (leaf1) leaf1->AddToList(list);
		if (leaf2) leaf2->AddToList(list);
		return;
	}*/
	
	std::string levelName;
	for (int i = 0; i < level; ++i)
		levelName += "   ";
	
	//std::cout << levelName << this << std::endl;
	
	scalar location = plane.getPointSide(position);
	
	if (location > 0.0) {
	//	std::cout << levelName << "   front: " << std::endl;
		if (back) back->Traverse(position, list, level + 1);
		if (leaf1) leaf1->AddToList(list);
		if (leaf2) leaf2->AddToList(list);
		if (front) front->Traverse(position, list, level + 1);
	}
	else if (location < 0.0) {
	//	std::cout << levelName << "   back: " << std::endl;
		if (front) front->Traverse(position, list, level + 1);
		if (leaf1) leaf1->AddToList(list);
		if (leaf2) leaf2->AddToList(list);
		if (back) back->Traverse(position, list, level + 1);
	}
	else {
	//	std::cout << levelName << "   intersect: " << std::endl;
		if (back) back->Traverse(position, list, level + 1);
		if (front) front->Traverse(position, list, level + 1);
	}
}

void BSPNode::Print(std::string name, int level) {
	std::string levelName;
	for (int i = 0; i < level; ++i)
		levelName += "   ";
	
	std::cout << levelName << name << " node" << std::endl;
	if (leaf1) leaf1->Print(level + 1);
	if (leaf2) leaf2->Print(level + 1);
	if (front) front->Print("front", level + 1);
	if (back) back->Print("back", level + 1);
	
	
}

void BSPLeaf::Print(int level) {
	std::string levelName;
	for (int i = 0; i < level; ++i)
		levelName += "   ";

	std::cout << levelName << "leaf " << index << ": " << surfaces.size() << " surfaces:" << std::endl;
	
	for (int i = 0; i < faces.size(); ++i)
		std::cout << levelName << "   " << faces[i] << std::endl;
	
}

