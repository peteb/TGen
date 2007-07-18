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
	void AddSurfaces(RenderList & list) const;
	
	const std::string & getName() const;
	void setPosition(const TGen::Vector3 & position);
	void setOrientation(const TGen::Vector3 & orientation);
	const TGen::Vector3 & getPosition() const;
	const TGen::Vector3 & getOrientation() const;
	const TGen::AABB & getAABB() const;
	scalar getBS() const;
	
protected:
	bool updated;
	TGen::AABB boundingBox;
	scalar boundingSphere;
	
private:
	typedef std::vector<Surface> SurfaceList;
	typedef std::vector<SceneNode *> SceneNodeList;
	
	SurfaceList surfaces;
	SceneNodeList children;
	std::string name;
	SceneNode * parent;
	
	TGen::Vector3 position, orientation, up;
	TGen::Matrix4x4 transform;
};


#endif // !_THEQUAKER_SCENENODE_H
