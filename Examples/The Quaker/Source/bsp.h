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

class BSPNode;

class BSPGeometry : public Geometry {
public:
	BSPGeometry(BSPNode & tree, bool wire);
	~BSPGeometry();
	
	void PrepareRender(TGen::Renderer & renderer) const;
	void Render(TGen::Renderer & renderer) const;
	TGen::Vector3 getMax() const;
	TGen::Vector3 getMin() const;
	
	int startIndex, numIndices;
	BSPNode & tree;
	TGen::IndexBuffer * ib;
	TGen::VertexBuffer * vb;
	bool wire;
	
	std::vector<std::pair<TGen::IndexBuffer *, int> > multidraw;
};

class BSPLeaf {
public:	
	
	void AddToList(RenderList & list);
	void AddSurface(const Surface & surf);
	
	typedef std::vector<Surface> SurfaceList;
	SurfaceList surfaces;
};

class BSPNode {
public:	
	BSPNode();

	void Traverse(const TGen::Vector3 & position, RenderList & list);
	
	TGen::Plane3 plane;
	BSPNode * front, * back;
	BSPLeaf * leaf1, * leaf2;
	TGen::Vector3 min, max;
	TGen::AABB aabb;
};

class BSPTree : public SceneNode {
public:
	BSPTree(const std::string & name);
	~BSPTree();
	
	void Accept(SceneNodeVisitor & visitor);
	void getNodes(const Camera & camera, RenderList & list);
	
	
	TGen::VertexBuffer * vb;
	TGen::IndexBuffer * ib;
	
	BSPNode * root;
	
	//typedef std::map<TGen::Material *, BSPGeometry *> GeometryMap;
	//GeometryMap geometryPerMaterial;
};


#endif // !_THEQUAKER_BSP_H
