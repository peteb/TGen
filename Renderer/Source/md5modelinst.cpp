/*
 *  md5modelinst.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/10/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "md5modelinst.h"
#include "md5meshinst.h"

TGen::MD5::ModelInstance::ModelInstance(const std::string & name, 
													 const std::string & materialNamePostfix, 
													 TGen::MD5::Model & base)
	: TGen::NewModelInstance(name, "", materialNamePostfix)
	, base(base)
{
	
}

TGen::MD5::ModelInstance::~ModelInstance() {
	
}

bool TGen::MD5::ModelInstance::isPureInstance() const {
	return true;
}

void TGen::MD5::ModelInstance::addMesh(TGen::MD5::MeshInstance * mesh) {
	meshes.push_back(mesh);
}

int TGen::MD5::ModelInstance::getNumMeshes() const {
	return meshes.size();
}

TGen::NewMeshInstance * TGen::MD5::ModelInstance::getMesh(int num) {
	return meshes[num];
}

void TGen::MD5::ModelInstance::fillFaces(TGen::RenderList & list, TGen::Material * overridingMaterial, TGen::SceneNode const * node) {

}
