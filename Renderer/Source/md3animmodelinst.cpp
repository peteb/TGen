/*
 *  md3animmodelinst.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/3/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "md3animmodelinst.h"
#include "md3animmodel.h"
#include "md3animmeshinst.h"
#include "renderlist.h"

TGen::MD3::AnimatingModelInstance::AnimatingModelInstance(const std::string & name, TGen::MD3::AnimatingModel & base) 
	: TGen::NewModelInstance(name)
	, base(base)
{
	
}

TGen::MD3::AnimatingModelInstance::~AnimatingModelInstance() {
	for (int i = 0; i < meshes.size(); ++i)
		delete meshes[i];
}

bool TGen::MD3::AnimatingModelInstance::isPureInstance() const {
	return true;
}

void TGen::MD3::AnimatingModelInstance::update() {
	static float num = 0.0f;
	
	for (int i = 0; i < meshes.size(); ++i)
		meshes[i]->updateVertices(int(num));
	
	num += 0.1f;
	if (num > 200.0)
		num = 0.0;
}

TGen::ModelJoint TGen::MD3::AnimatingModelInstance::getJoint(const std::string & name) const {
	return base.getJoint(name);
}

void TGen::MD3::AnimatingModelInstance::linkMaterial(TGen::MaterialSource & source) {
	for (int i = 0; i < meshes.size(); ++i)
		meshes[i]->linkMaterial(source);
}

void TGen::MD3::AnimatingModelInstance::unlinkMaterial() {
	for (int i = 0; i < meshes.size(); ++i)
		meshes[i]->unlinkMaterial();	
}

void TGen::MD3::AnimatingModelInstance::fillFaces(TGen::RenderList & list, TGen::SceneNode const * node) {
	for (int i = 0; i < meshes.size(); ++i) {
		list.addFace(TGen::NewFace(meshes[i], meshes[i]->getMaterial(), node));
	}	
}

int TGen::MD3::AnimatingModelInstance::getNumMeshes() const {
	return meshes.size();
}

TGen::NewMeshInstance * TGen::MD3::AnimatingModelInstance::getMesh(int num) {
	return meshes[num];
}

void TGen::MD3::AnimatingModelInstance::addMesh(TGen::MD3::AnimatingMeshInstance * mesh) {
	meshes.push_back(mesh);
}
