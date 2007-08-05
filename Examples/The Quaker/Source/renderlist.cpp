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

bool Sorter::operator() (const Surface * s1, const Surface * s2) {
	return s1->getMaterial() > s2->getMaterial();
}

RenderList::RenderList() {
	surfaces.reserve(200000);
}

RenderList::~RenderList() {
	
}

void RenderList::Render(TGen::Renderer & renderer, const Camera & camera) {
	std::cout << "surfaces to render: " << surfaces.size() << std::endl;
	
	TGen::Matrix4x4 baseMat = camera.getTransform();
	renderer.setTransform(TGen::TransformProjection, camera.getProjection());
	
	SceneNode * lastNode = NULL;
	TGen::Material * lastMaterial = NULL;
	
	static float surfers = 0.0f;
	surfers += 0.01f;
	
	int surfs = surfers;
	
	if (surfs > surfaces.size())
		surfs = surfaces.size();
	
	for (int i = 0; i < surfs; ++i) {
		SceneNode * thisNode = surfaces[i]->getSceneNode();
		if (thisNode != lastNode && thisNode) {
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
	surfaces.clear();  // Testa nu: vanliga material-batchningen men transparenter blir egna objekt. sen sorteras allt med radix
}

void RenderList::Sort() {
	std::sort(surfaces.begin(), surfaces.end(), Sorter());	
	
	/*for (int i = 0; i < surfaces.size(); ++i) {
		std::cout << " ------ " << surfaces[i]->getMaterial()->getName() << std::endl;
	}*/
	
}

