/*
 *  scene.cpp
 *  The Quaker
 *
 *  Created by Peter Backman on 7/18/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "scene.h"
#include "scenenode.h"
#include "camera.h"

Scene::Scene() 
	: root(NULL)
{
	
	root = new SceneNode("root");
}

Scene::~Scene() {
	delete root;
}

void Scene::Update(scalar dt) {
	if (root)
		root->Update(TGen::Matrix4x4::Identity, false);
}

void Scene::AddSurfaces(RenderList & renderList, const Camera & camera) {
	root->AddSurfaces(renderList);
}

SceneNode * Scene::getSceneRoot() {
	return root;
}
