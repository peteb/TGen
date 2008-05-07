/*
 *  md5model.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 1/19/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "md5model.h"
#include "md5modelinst.h"
#include "md5mesh.h"
#include "md5meshinst.h"

TGen::MD5::Model::Model(const std::string & name, TGen::VertexDataSource & dataSource) 
	: TGen::NewModel(name, "", "")
	, dataSource(dataSource)
{
	
}

TGen::MD5::Model::~Model() {
	
}

/*void TGen::MD5::Mesh::preRender(TGen::Renderer & renderer) const {
	
}

void TGen::MD5::Mesh::render(TGen::Renderer & renderer) const {
	
}

void TGen::MD5::Mesh::update(const TGen::Camera & camera, scalar distance, scalar time) {
	
}

TGen::Vector3 TGen::MD5::Mesh::getMax() const {
	return TGen::Vector3(1.0f, 1.0f, 1.0f);
}

TGen::Vector3 TGen::MD5::Mesh::getMin() const {
	return TGen::Vector3(-1.0f, -1.0f, -1.0f);
}

TGen::Vector3 TGen::MD5::Mesh::getOrigin() const {
	return TGen::Vector3(0.0f, 0.0f, 0.0f);
}

std::string TGen::MD5::Mesh::getDefaultMaterial() const {
	return "";
}
*/

TGen::NewModelInstance * TGen::MD5::Model::instantiate(TGen::VertexDataSource & source) {
	TGen::MD5::ModelInstance * newInstance = new TGen::MD5::ModelInstance(name + "_instance", "", *this);
	
	for (int i = 0; i < meshes.size(); ++i) {
		TGen::MD5::Mesh const & baseMesh = *meshes[i];
		TGen::MD5::MeshInstance * newMeshInstance = new TGen::MD5::MeshInstance(baseMesh.getMaterialName(), "", baseMesh);
		
		newMeshInstance->createVertexData(dataSource);
		newInstance->addMesh(newMeshInstance);
	}
	
	return newInstance;
}

bool TGen::MD5::Model::isPureInstance() const {
	return true;
}

void TGen::MD5::Model::addMeshData(TGen::MD5::Mesh * mesh) {
	meshes.push_back(mesh);
}
