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

TGen::NewModelInstance::NewModelInstance(const std::string & name)
	: name(name)
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
	throw TGen::RuntimeException("ModelInstance::linkMaterial", "no implementation! Did you dereference links?");	
}

void TGen::NewModelInstance::unlinkMaterial() {
	throw TGen::RuntimeException("ModelInstance::unlinkMaterial", "no implementation! Did you dereference links?");	
}

void TGen::NewModelInstance::fillFaces(TGen::RenderList & list, TGen::SceneNode const * node) {
	throw TGen::RuntimeException("ModelInstance::fillFaces", "no implementation! Did you dereference links?");	
}

int TGen::NewModelInstance::getNumMeshes() const {
	throw TGen::RuntimeException("ModelInstance::getNumMeshes", "no implementation! Did you dereference links?");
}

TGen::NewMeshInstance * TGen::NewModelInstance::getMesh(int num) {
	throw TGen::RuntimeException("ModelInstance::getMesh", "no implementation! Did you dereference links?");
}
