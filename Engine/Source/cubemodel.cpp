/*
 *  cubemodel.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/26/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "cubemodel.h"
#include "cubemesh.h"
#include "metacreator.h"

TGen::Engine::CubeModel::CubeModel(const std::string & name, const std::string & materialName, const std::string & materialNamePostfix, const TGen::Vector3 & min, const TGen::Vector3 & max, TGen::VertexDataSource & dataSource) 
	: TGen::NewModel(name, "bullet", materialNamePostfix)
	, min(min)
	, max(max)
	, dataSource(dataSource)
{

}

TGen::Engine::CubeModel::~CubeModel() {
	
}

TGen::Engine::CubeModel * TGen::Engine::CubeModel::instantiate(TGen::VertexDataSource & source) const {
	return const_cast<TGen::Engine::CubeModel *>(this);
}

bool TGen::Engine::CubeModel::isPureInstance() const {
	return false;
}

TGen::Vector3 TGen::Engine::CubeModel::getMax() const {
	return max;
}

TGen::Vector3 TGen::Engine::CubeModel::getMin() const {
	return min;
}

TGen::NewMeshInstance * TGen::Engine::CubeModel::getMesh(int num) {
	return meshes[num];
}

int TGen::Engine::CubeModel::getNumMeshes() const {
	return meshes.size();
}


void TGen::Engine::CubeModel::addMesh(TGen::Engine::CubeMesh * instance) {
	meshes.push_back(instance);
}

void TGen::Engine::CubeModel::fillFaces(TGen::RenderList & list,  TGen::Material * overridingMaterial, const TGen::SceneNode * node, const TGen::ModelRenderProperties * renderProperties) {
	for (int i = 0; i < meshes.size(); ++i) {
		list.addFace(TGen::NewFace(meshes[i], (overridingMaterial ? overridingMaterial : meshes[i]->getMaterial()), node, renderProperties));		
	}
}

void TGen::Engine::CubeModel::linkMaterial(TGen::MaterialSource & source) {
	TGen::NewModelInstance::linkMaterial(source);
		
	for (int i = 0; i < meshes.size(); ++i)
		meshes[i]->linkMaterial(source);	
}

void TGen::Engine::CubeModel::unlinkMaterial() {
	TGen::NewModelInstance::unlinkMaterial();
	
	for (int i = 0; i < meshes.size(); ++i)
		meshes[i]->unlinkMaterial();		
}

void TGen::Engine::CubeModel::writeMeta(uint metaType, const TGen::Matrix4x4 & transform, TGen::VertexStream & stream) {
	if (metaType == TGen::MetaAxis) {
		TGen::Engine::MetaCreator mc;
	
		mc.writeAxes(transform, stream);
	}
}

TGen::Engine::CubeModel * TGen::Engine::CubeModel::clone() const {
	return instantiate(dataSource);
}

