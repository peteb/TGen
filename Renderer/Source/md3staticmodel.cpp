/*
 *  md3staticmodel.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/23/07.
 *  Renamed by Peter Backman on 2/3/08.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "md3staticmodel.h"
#include "md3mesh.h"
#include "renderlist.h"
#include "model_new.h"

TGen::MD3::StaticModel::StaticModel(const std::string & name)
	: TGen::NewModel(name)
{
		
}

TGen::MD3::StaticModel::~StaticModel() {
	for (int i = 0; i < meshes.size(); ++i)
		delete meshes[i];

}

bool TGen::MD3::StaticModel::isPureInstance() const {
	return false;
}

TGen::MD3::StaticModel * TGen::MD3::StaticModel::instantiate() {
	return this;
}

TGen::ModelJoint TGen::MD3::StaticModel::getJoint(const std::string & name) const {
	JointMap::const_iterator iter = joints.find(name);
	if (iter == joints.end())
		throw TGen::RuntimeException("Model::getJoint", "joint not found: '" + name + "'");
	
	return iter->second;
}

void TGen::MD3::StaticModel::addJoint(const std::string & name, const TGen::ModelJoint & joint) {
	if (joints.find(name) != joints.end())
		throw TGen::RuntimeException("MD3::Model::addJoint", "joint '" + name + "' already added");
	
	joints.insert(JointMap::value_type(name, joint));
}

int TGen::MD3::StaticModel::getNumMeshes() const {
	return meshes.size();
}

TGen::NewMeshInstance * TGen::MD3::StaticModel::getMesh(int num) {
	return meshes[num];
}

void TGen::MD3::StaticModel::addMesh(TGen::MD3::Mesh * instance) {
	meshes.push_back(instance);
}

void TGen::MD3::StaticModel::fillFaces(TGen::RenderList & list,  TGen::Material * overridingMaterial, TGen::SceneNode const * node) {
	for (int i = 0; i < meshes.size(); ++i) {
		list.addFace(TGen::NewFace(meshes[i], (overridingMaterial ? overridingMaterial : meshes[i]->getMaterial()), node));
	}
}

void TGen::MD3::StaticModel::linkMaterial(TGen::MaterialSource & source) {
	TGen::NewModelInstance::linkMaterial(source);

	for (int i = 0; i < meshes.size(); ++i)
		meshes[i]->linkMaterial(source);
}

void TGen::MD3::StaticModel::unlinkMaterial() {
	TGen::NewModelInstance::unlinkMaterial();
	
	for (int i = 0; i < meshes.size(); ++i)
		meshes[i]->unlinkMaterial();	
}

void TGen::MD3::StaticModel::update() {
	for (int i = 0; i < meshes.size(); ++i)
		meshes[i]->update();
}

TGen::Vector3 TGen::MD3::StaticModel::getMax() const {
	return TGen::Vector3(1.0f, 1.0f, 1.0f);
}

TGen::Vector3 TGen::MD3::StaticModel::getMin() const {
	return TGen::Vector3(-1.0f, -1.0f, -1.0f);
}

TGen::Vector3 TGen::MD3::StaticModel::getOrigin() const {
	return TGen::Vector3::Zero;
}

// max/min/origin/radius i model
