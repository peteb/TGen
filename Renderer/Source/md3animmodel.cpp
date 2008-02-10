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
#include "md3animmeshsingle.h"
#include "md3animmeshdouble.h"
#include "renderlist.h"
#include "model_new.h"

TGen::MD3::AnimatingModel::AnimatingModel(const std::string & name, TGen::VertexDataSource & dataSource)
	: TGen::NewModel(name, "", "")
	, dataSource(dataSource)
{
	setInterpolatingMode(TGen::MD3::CPU);
}

TGen::MD3::AnimatingModel::~AnimatingModel() {
	for (int i = 0; i < meshes.size(); ++i)
		delete meshes[i];
	
}

bool TGen::MD3::AnimatingModel::isPureInstance() const {
	return true;
}

// TODO: ett fallbackmaterial ska kunna definieras ... varför? glömt bort
//       sen ska md5 blir nya modellsystemet...

TGen::NewModelInstance * TGen::MD3::AnimatingModel::instantiate() {
	TGen::MD3::AnimatingModelInstance * newInstance = NULL;
	
	switch (interpolatingMode) {
		case TGen::MD3::CPU:
			newInstance = new TGen::MD3::AnimatingModelInstance(name + "_instance", "", *this);
			break;
			
		case TGen::MD3::GPU:
			newInstance = new TGen::MD3::AnimatingModelInstance(name + "_instance", "gpuinterpol", *this);
			break;			
			
		default:
			throw TGen::RuntimeException("MD3::AnimatingModel::instantiate", "interpolating mode invald");
	}
	
	for (int i = 0; i < meshes.size(); ++i) {
		TGen::MD3::AnimatingMesh const & mesh = *meshes[i];
		TGen::MD3::AnimatingMeshInstance * newMeshInstance = NULL;
		
		switch (interpolatingMode) {
			case TGen::MD3::CPU:
				newMeshInstance = new TGen::MD3::AnimatingMeshSingle(mesh.getMaterialName(), mesh);				
				break;
				
			case TGen::MD3::GPU:
				newMeshInstance = new TGen::MD3::AnimatingMeshDouble(mesh.getMaterialName(), mesh);				
				break;
				
			default:
				throw TGen::RuntimeException("MD3::AnimatingModel::instantiate", "interpolating mode invald");
		}
		
		newMeshInstance->createVertexData(dataSource);
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

void TGen::MD3::AnimatingModel::setInterpolatingMode(TGen::MD3::InterpolatingMode mode) {
	interpolatingMode = mode;
}


