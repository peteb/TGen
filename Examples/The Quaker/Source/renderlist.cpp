/*
 *  renderlist.cpp
 *  The Quaker
 *
 *  Created by Peter Backman on 7/18/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "renderlist.h"
#include "scenenode.h"
#include "camera.h"

RenderList::RenderList() {
	surfaces.reserve(200000);
}

RenderList::~RenderList() {
	
}

void RenderList::Render(TGen::Renderer & renderer, const Camera & camera) {
	//std::cout << "surfaces to render: " << surfaces.size() << std::endl;
	
	TGen::Matrix4x4 baseMat = camera.getTransform();
	renderer.setTransform(TGen::TransformProjection, camera.getProjection());
	
	SceneNode * lastNode = NULL;
	TGen::Material * lastMaterial = NULL;
	
	for (int i = 0; i < surfaces.size(); ++i) {
		SceneNode * thisNode = surfaces[i]->getSceneNode();
		if (thisNode != lastNode) {
			renderer.setTransform(TGen::TransformWorldView, baseMat * thisNode->getTransform());
			lastNode = thisNode;
		}
		
		TGen::Material * mat = surfaces[i]->getMaterial();
		mat->Render(renderer, *surfaces[i], "default", 9, NULL);
	}
}

void RenderList::AddSurface(const Surface * surface) {
	surfaces.push_back(surface);
}

void RenderList::Clear() {
	surfaces.clear();
}


