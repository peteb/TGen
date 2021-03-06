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

TGen::RenderFiller::RenderFiller(TGen::RenderList & list, const TGen::Camera & camera)
	: TGen::SceneNode::Walker(TGen::SceneNode::WalkerFollowVisibility)
	, list(list)
	, camera(camera)
{
	
}

bool TGen::RenderFiller::pre(TGen::SceneNode & node) const {
	node.fillUser(list, camera);
	node.fillMeta(list, camera);
	return node.fillFaces(list, camera);
}

void TGen::RenderFiller::post(TGen::SceneNode & node) const {
	
}

