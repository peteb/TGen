/*
 *  scene.h
 *  The Quaker
 *
 *  Created by Peter Backman on 7/18/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _THEQUAKER_SCENE_H
#define _THEQUAKER_SCENE_H

#include <tgen_core.h>

class SceneNode;
class RenderList;
class Camera;

class Scene {
public:	
	Scene();
	~Scene();

	void Update(scalar dt);
	//void AddSurfaces(RenderList & renderList, const Camera & camera);
	SceneNode * getSceneRoot();
	
private:
	SceneNode * root;
};

#endif // !_THEQUAKER_SCENE_H
