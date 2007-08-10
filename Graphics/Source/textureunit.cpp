/*
 *  textureunit.cpp
 *  TGen Graphics
 *
 *  Created by Peter Backman on 8/5/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "textureunit.h"

TGen::TextureUnit::TextureUnit(int unit, TGen::Texture * texture) 
	: unit(unit)
	, texture(texture)
	, transformed(false)
	, transform(TGen::Matrix4x4::Identity)
	, genU(TGen::TextureCoordGenBase)
	, genV(TGen::TextureCoordGenBase)
	, textureType(0) 
{
}

