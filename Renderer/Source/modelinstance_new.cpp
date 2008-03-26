/*
 *  modelinstance_new.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/3/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "modelinstance_new.h"
#include "meshinstance_new.h"
#include "renderlist.h"
#include "materialsource.h"

TGen::NewModelInstance::NewModelInstance(const std::string & name, 
													  const std::string & materialName, 
													  const std::string & materialNamePostfix)
	: name(name)
	, materialName(materialName)
	, material(NULL)
	, materialNamePostfix(materialNamePostfix)
{
}

TGen::NewModelInstance::~NewModelInstance() {

}

std::string TGen::NewModelInstance::getName() const {
	return name;
}

void TGen::NewModelInstance::update() {
	
}

TGen::ModelJoint TGen::NewModelInstance::getJoint(const std::string & name) const {
	throw TGen::RuntimeException("ModelInstance::getJoint", "no implementation! Did you dereference links?");
}

void TGen::NewModelInstance::linkMaterial(TGen::MaterialSource & source) {
	if (!materialName.empty())
		material = source.getMaterial(getOverridingMaterialName());
	else
		material = NULL;
	
	//throw TGen::RuntimeException("ModelInstance::linkMaterial", "no implementation! Did you dereference links?");	
}

void TGen::NewModelInstance::unlinkMaterial() {
	material = NULL;
	//throw TGen::RuntimeException("ModelInstance::unlinkMaterial", "no implementation! Did you dereference links?");	
}

TGen::Material * TGen::NewModelInstance::getOverridingMaterial() {
	return material;
}

std::string TGen::NewModelInstance::getOverridingMaterialName() const {
	if (!materialNamePostfix.empty())
		return materialName + "_" + materialNamePostfix;
	
	return materialName;
}

std::string TGen::NewModelInstance::getMaterialNamePostfix() const {
	return materialNamePostfix;
}

void TGen::NewModelInstance::fillFaces(TGen::RenderList & list, TGen::Material * overridingMaterial, const TGen::SceneNode * node) {
	throw TGen::RuntimeException("ModelInstance::fillFaces", "no implementation! Did you dereference links?");	
}

int TGen::NewModelInstance::getNumMeshes() const {
	throw TGen::RuntimeException("ModelInstance::getNumMeshes", "no implementation! Did you dereference links?");
}

TGen::NewMeshInstance * TGen::NewModelInstance::getMesh(int num) {
	throw TGen::RuntimeException("ModelInstance::getMesh", "no implementation! Did you dereference links?");
}
