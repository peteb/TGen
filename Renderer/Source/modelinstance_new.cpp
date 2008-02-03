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
	for (int i = 0; i < meshes.size(); ++i)
		delete meshes[i];
}

std::string TGen::NewModelInstance::getName() const {
	return name;
}


void TGen::NewModelInstance::linkMaterial(TGen::MaterialSource & source) {
	for (int i = 0; i < meshes.size(); ++i)
		meshes[i]->linkMaterial(source);
}

void TGen::NewModelInstance::unlinkMaterial() {
	for (int i = 0; i < meshes.size(); ++i)
		meshes[i]->unlinkMaterial();	
}

void TGen::NewModelInstance::update() {
	for (int i = 0; i < meshes.size(); ++i)
		meshes[i]->update();
}

void TGen::NewModelInstance::addMeshInstance(TGen::NewMeshInstance * instance) {
	meshes.push_back(instance);
}

void TGen::NewModelInstance::fillFaces(TGen::RenderList & list, TGen::SceneNode const * node) {
	for (int i = 0; i < meshes.size(); ++i) {
		list.addFace(TGen::NewFace(meshes[i], meshes[i]->getMaterial(), node));
	}
}

TGen::NewModelInstance::MeshList & TGen::NewModelInstance::getMeshes() {
	return meshes;
}

TGen::ModelJoint TGen::NewModelInstance::getJoint(const std::string & name) const {
	throw TGen::RuntimeException("ModelInstance::getJoint", "no implementation! Did you dereference links?");
}
