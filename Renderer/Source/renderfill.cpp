/*
 *  renderfill.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/14/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "renderfill.h"
#include "renderlist.h"

TGen::RenderFiller::RenderFiller(TGen::RenderList & list)
	: list(list)
{
	
}

bool TGen::RenderFiller::pre(TGen::SceneNode & node) const {
	for (int i = 0; i < node.getFaces().size(); ++i) {
		list.addFace(node.getFaces()[i]);
	}
	
	return true;
}

void TGen::RenderFiller::post(TGen::SceneNode & node) const {
	
}

