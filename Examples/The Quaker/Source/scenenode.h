/*
 *  scenenode.h
 *  The Quaker
 *
 *  Created by Peter Backman on 7/16/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _THEQUAKER_SCENENODE_H
#define _THEQUAKER_SCENENODE_H

#include "surface.h"
#include <tgen_math.h>

class RenderList;
class Camera;

class SceneNodeVisitor {
public:
	virtual ~SceneNodeVisitor() {}
	
	virtual void Visit(SceneNode & node) abstract;
};

class SceneNode {
public:
	SceneNode(const std::string & name);
	virtual ~SceneNode();
	
	const TGen::Matrix4x4 & getTransform() const;
	virtual void Update(const TGen::Matrix4x4 & parent, bool parentUpdated);

	void AddSurface(const Surface & surface);
	void AddChild(SceneNode * node);
	void RemoveChild(SceneNode * node);
	void Attached(SceneNode * parent);
	void Detached();
	virtual void CalculateBV();
	void CalculateSurfacesObjectBV();
	void CalculateWorldBV();
	
//	void AddSurfaces(RenderList & list, const Camera & camera) const;
	void Accept(SceneNodeVisitor & visitor);
	
	const std::string & getName() const;
	void setPosition(const TGen::Vector3 & position);
	void setOrientation(const TGen::Vector3 & orientation);
	void setUp(const TGen::Vector3 & up);
	
	const TGen::Vector3 & getPosition() const;
	const TGen::Vector3 & getOrientation() const;
	const TGen::Vector3 & getUp() const;
	
	TGen::Vector3 getWorldPosition() const;
	TGen::Vector3 getWorldOrientation() const;
	
	const TGen::AABB & getWorldAABB() const;
	const TGen::AABB & getObjectAABB() const;	
	const TGen::Sphere & getObjectBS() const;
	const TGen::Sphere & getWorldBS() const;
	
	friend class SceneNodeRenderer;
	
protected:
	bool updated;
	TGen::AABB objectBoundingBox, worldBoundingBox;
	TGen::Sphere objectBoundingSphere, worldBoundingSphere;
	TGen::Matrix4x4 transform;
	
private:
	typedef std::vector<Surface> SurfaceList;
	typedef std::vector<SceneNode *> SceneNodeList;
	
	SurfaceList surfaces;
	SceneNodeList children;
	std::string name;
	SceneNode * parent;
	TGen::Vector3 position, orientation, up;	
};


#endif // !_THEQUAKER_SCENENODE_H
