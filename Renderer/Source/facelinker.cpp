/*
 *  facelinker.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "facelinker.h"
#include "materialsource.h"
#include "face.h"
#include "modelinstance_new.h"

TGen::FaceLinker::FaceLinker(TGen::MaterialSource & source)
	: source(source)
{		
}

bool TGen::FaceLinker::pre(TGen::SceneNode & node) const {
	node.linkMaterials(source);

	for (int i = 0; i < node.getFaces().size(); ++i) {
		node.getFaces()[i].linkMaterial(source);
	}
	
	for (int i = 0; i < node.getModels().size(); ++i) {
		node.getModels()[i]->linkMaterial(source);
		TGen::DerefRes(node.getModels()[i])->linkMaterial(source);
	}
	
	return true;
}

void TGen::FaceLinker::post(TGen::SceneNode & node) const {
	// MeshGeomLinkList
}

