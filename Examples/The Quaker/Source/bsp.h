/*
 *  bsp.h
 *  The Quaker
 *
 *  Created by Peter Backman on 7/21/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _THEQUAKER_BSP_H
#define _THEQUAKER_BSP_H

#include "scenenode.h"

class BSPTree;

/*class MaterialBuffer : public Geometry {
public:
	MaterialBuffer(BSPTree & tree);
	~MaterialBuffer();
	
	void PrepareRenderer(TGen::Renderer & renderer) const;
	void Render(TGen::Renderer & renderer) const;
	TGen::Vector3 getMax() const;
	TGen::Vector3 getMin() const;

	BSPTree & tree;	
	TGen::IndexBuffer * ib;
	int start, indices;	
};*/

class BSPGeometry : public Geometry {
public:
	BSPGeometry(BSPTree & tree, bool wire);
	~BSPGeometry();
	
	void PrepareRender(TGen::Renderer & renderer) const;
	void Render(TGen::Renderer & renderer) const;
	TGen::Vector3 getMax() const;
	TGen::Vector3 getMin() const;
	
	int startIndex, numIndices;
	BSPTree & tree;
	TGen::IndexBuffer * ib;
	TGen::VertexBuffer * vb;
	bool wire;
	
	std::vector<std::pair<TGen::IndexBuffer *, int> > multidraw;
};

class BSPTree : public SceneNode {
public:
	BSPTree(const std::string & name);
	~BSPTree();
	
	TGen::VertexBuffer * vb;

	//typedef std::map<TGen::Material *, BSPGeometry *> GeometryMap;
	//GeometryMap geometryPerMaterial;
};


#endif // !_THEQUAKER_BSP_H
