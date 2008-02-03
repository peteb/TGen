/*
 *  md3animmeshinst.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/3/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "md3animmeshinst.h"

TGen::MD3::AnimatingMeshInstance::AnimatingMeshInstance(const std::string & materialName, TGen::MD3::AnimatingMesh & base) 
	: TGen::NewMeshInstance(materialName)
	, base(base)
{

}

TGen::MD3::AnimatingMeshInstance::~AnimatingMeshInstance() {

}

void TGen::MD3::AnimatingMeshInstance::preRender(TGen::Renderer & renderer) const {
	
}

void TGen::MD3::AnimatingMeshInstance::render(TGen::Renderer & renderer) const {
	
}

