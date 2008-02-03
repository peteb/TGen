/*
 *  meshinstance_new.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/3/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "meshinstance_new.h"
#include "materialsource.h"

TGen::NewMeshInstance::NewMeshInstance(const std::string & materialName)
	: materialName(materialName)
	, material(NULL)
{
	
}

TGen::NewMeshInstance::~NewMeshInstance() {
	
}

void TGen::NewMeshInstance::linkMaterial(TGen::MaterialSource & source) {
	//if (!materialName.empty())
		material = source.getMaterial(materialName);
}

void TGen::NewMeshInstance::unlinkMaterial() {
	material = NULL;
}

TGen::NewMeshInstance * TGen::NewMeshInstance::instantiate() const {
	throw TGen::RuntimeException("NewMeshInstance::instantiate", "can't instantiate an instance!");
}

TGen::Vector3 TGen::NewMeshInstance::getMax() const {
	return max;
}

TGen::Vector3 TGen::NewMeshInstance::getMin() const {
	return min;
}

TGen::Vector3 TGen::NewMeshInstance::getOrigin() const {
	return origin;
}

TGen::Material * TGen::NewMeshInstance::getMaterial() const {
	return material;
}

std::string TGen::NewMeshInstance::getMaterialName() const {
	return materialName;
}
