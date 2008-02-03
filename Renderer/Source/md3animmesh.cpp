/*
 *  md3animmesh.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/3/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "md3animmesh.h"

TGen::MD3::AnimationFrame::AnimationFrame(const std::string & name)
	: name(name)
{
}

TGen::MD3::AnimatingMesh::AnimatingMesh(const std::string & materialName)
	: materialName(materialName)
{
}

void TGen::MD3::AnimatingMesh::addAnimationFrame(TGen::MD3::AnimationFrame * frame) {
	frames.push_back(frame);
}

std::string TGen::MD3::AnimatingMesh::getMaterialName() const {
	return materialName;
}
