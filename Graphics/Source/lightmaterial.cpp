/*
 *  lightmaterial.cpp
 *  TGen Graphics
 *
 *  Created by Peter Backman on 1/7/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include "lightmaterial.h"

TGen::LightMaterial::LightMaterial() 
	: shininess(20.0f)
	, diffuse(TGen::Color::White)
	, specular(TGen::Color(0.5f, 0.5f, 0.5f)) 
{
}
