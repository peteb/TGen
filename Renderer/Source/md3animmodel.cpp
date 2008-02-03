/*
 *  md3animmodel.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/3/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "md3animmodel.h"
#include "md3animmesh.h"
#include "md3animmodelinst.h"
#include "md3animmeshinst.h"
#include "renderlist.h"
#include "model_new.h"

TGen::MD3::AnimatingModel::AnimatingModel(const std::string & name)
	: TGen::NewModel(name)
{
	
}

TGen::MD3::AnimatingModel::~AnimatingModel() {
	for (int i = 0; i < meshes.size(); ++i)
		delete meshes[i];
	
}

bool TGen::MD3::AnimatingModel::isPureInstance() const {
	return true;
}

TGen::NewModelInstance * TGen::MD3::AnimatingModel::instantiate() {
	TGen::MD3::AnimatingModelInstance * newInstance = new TGen::MD3::AnimatingModelInstance(name + "_instance", *this);
	
	for (int i = 0; i < meshes.size(); ++i) {
		TGen::MD3::AnimatingMeshInstance * newMeshInstance = new TGen::MD3::AnimatingMeshInstance(meshes[i]->getMaterialName(), *meshes[i]);
		
		newInstance->addMesh(newMeshInstance);
	}
	
	return newInstance;
}

TGen::ModelJoint TGen::MD3::AnimatingModel::getJoint(const std::string & name) const {
	JointMap::const_iterator iter = joints.find(name);
	if (iter == joints.end())
		throw TGen::RuntimeException("Model::getJoint", "joint not found: '" + name + "'");
	
	return iter->second;
}

void TGen::MD3::AnimatingModel::addJoint(const std::string & name, const TGen::ModelJoint & joint) {
	if (joints.find(name) != joints.end())
		throw TGen::RuntimeException("MD3::Model::addJoint", "joint '" + name + "' already added");
	
	joints.insert(JointMap::value_type(name, joint));
}

void TGen::MD3::AnimatingModel::addMeshData(TGen::MD3::AnimatingMesh * mesh) {
	meshes.push_back(mesh);
}


