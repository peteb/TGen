/*
 *  face.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "face.h"
#include "materialsource.h"

TGen::Face::Face()
	: material(NULL)
{
		
}

TGen::Face::~Face() {
	
}

TGen::Vector3 TGen::Face::getMin() const {
	return TGen::Vector3(0.0f, 0.0f, 0.0f);
}

TGen::Vector3 TGen::Face::getMax() const {
	return TGen::Vector3(0.0f, 0.0f, 0.0f);
}

void TGen::Face::linkMaterial(TGen::MaterialSource & source) {
	if (!materialName.empty())
		material = source.getMaterial(materialName);
}

TGen::Material * TGen::Face::getMaterial() const {
	return material;
}
