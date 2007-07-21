/*
 *  scenenoderenderer.h
 *  The Quaker
 *
 *  Created by Peter Backman on 7/19/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _THEQUAKER_SCENENODERENDERER_H
#define _THEQUAKER_SCENENODERENDERER_H

#include "scenenode.h"

class RenderList;
class Camera;

class AABBRenderer : public SceneNodeVisitor {
public:
	AABBRenderer(TGen::Batch<TGen::Vertex3<float>, 2> & batch, Camera & camera);
	
	void Visit(SceneNode & node);
	
private:
	TGen::Batch<TGen::Vertex3<float>, 2> & batch;
	Camera & camera;
};

class SceneNodeRenderer : public SceneNodeVisitor {
public:
	SceneNodeRenderer(RenderList & list, Camera & camera, int maxSurfaces);
	
	void Visit(SceneNode & node);
	
private:
	RenderList & list;
	Camera & camera;
	int maxSurfaces;
};

#endif // !_THEQUAKER_SCENENODERENDERER_H
