/*
 *  q3mapmodel.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/28/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include "q3mapmodel.h"
#include "q3mapmesh.h"

TGen::Engine::Q3MapModel::Q3MapModel(const std::string & name)
	: TGen::NewModel(name, "", "")
{

}


bool TGen::Engine::Q3MapModel::isPureInstance() const {
	return false;
}

TGen::NewModelInstance * TGen::Engine::Q3MapModel::instantiate(TGen::VertexDataSource & source) const {
	for (int i = 0; i < meshes.size(); ++i) {
		meshes[i]->instantiate(source);
	}
	
	return const_cast<TGen::Engine::Q3MapModel *>(this);
}

void TGen::Engine::Q3MapModel::fillFaces(TGen::RenderList & list, TGen::Material * overridingMaterial, const TGen::SceneNode * node) {
	for (int i = 0; i < meshes.size(); ++i) {		
		list.addFace(TGen::NewFace(meshes[i], (overridingMaterial ? overridingMaterial : meshes[i]->getMaterial()), node, NULL));
	}
}


void TGen::Engine::Q3MapModel::addMesh(TGen::Engine::Q3MapMesh * mesh) {
	meshes.push_back(mesh);
}



void TGen::Engine::Q3MapModel::linkMaterial(TGen::MaterialSource & source) {
	for (int i = 0; i < meshes.size(); ++i)
		meshes[i]->linkMaterial(source);
}

