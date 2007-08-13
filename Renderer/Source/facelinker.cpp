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

TGen::FaceLinker::FaceLinker(TGen::MaterialSource & source)
	: source(source)
{		
}

bool TGen::FaceLinker::pre(TGen::SceneNode & node) const {
	for (int i = 0; i < node.getFaces().size(); ++i) {
		node.getFaces()[i]->linkMaterial(source);
	}
	
	return true;
}

void TGen::FaceLinker::post(TGen::SceneNode & node) const {
	
}

