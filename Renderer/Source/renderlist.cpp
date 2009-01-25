/*
 *  renderlist.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/14/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "renderlist.h"

void TGen::RenderList::setMaterialOverride(TGen::MaterialOverride * override, int param) {
	this->override = override;
	this->overrideParam = param;
}

void TGen::RenderList::setMaterial(TGen::Material * material) {
	this->materialOverride = material;
}

void TGen::RenderList::setShaderMode(int mode) {
	shaderMode = mode;
}
