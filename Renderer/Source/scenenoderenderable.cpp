/*
 *  scenenoderenderable.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 4/4/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "scenenoderenderable.h"
#include "scenenode.h"
#include "modelinstance_new.h"
#include "meshinstance_new.h"

TGen::SceneNodeRenderable::SceneNodeRenderable(TGen::SceneNode & node)
	: node(node)
{

}

void TGen::SceneNodeRenderable::preRender(TGen::Renderer & renderer) const {
	TGen::SceneNode::ModelInstanceList & models = node.getModels();
	
	for (int i = 0; i < models.size(); ++i) {
		for (int a = 0; a < TGen::DerefRes(models[i])->getNumMeshes(); ++a) {
			TGen::NewMeshInstance * mesh = TGen::DerefRes(models[i])->getMesh(a);
			mesh->preRender(renderer);
		}
	}	
}

void TGen::SceneNodeRenderable::render(TGen::Renderer & renderer) const {
	TGen::SceneNode::ModelInstanceList & models = node.getModels();
	
	for (int i = 0; i < models.size(); ++i) {
		for (int a = 0; a < TGen::DerefRes(models[i])->getNumMeshes(); ++a) {
			TGen::NewMeshInstance * mesh = TGen::DerefRes(models[i])->getMesh(a);
			mesh->render(renderer);
		}
	}
}

