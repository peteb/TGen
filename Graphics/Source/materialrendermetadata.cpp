/*
 *  materialrendermetadata.cpp
 *  TGen Graphics
 *
 *  Created by Peter Backman on 1/7/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include "materialrendermetadata.h"

TGen::MaterialRenderMetadata::MaterialRenderMetadata(const std::string & mode, int lod, TGen::Texture ** textureTypes, int shaderMode, TGen::ShaderVariableUpdater * varupdater, TGen::MaterialOverride * override)
	: mode(mode)
	, lod(lod)
	, textureTypes(textureTypes)
	, shaderMode(shaderMode)
	, varupdater(varupdater)
	, override(override)
{
	
}
