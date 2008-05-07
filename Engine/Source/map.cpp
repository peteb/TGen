/*
 *  map.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/29/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "map.h"
#include "mapmodel.h"

TGen::Engine::Map::Map(const std::string & name, const TGen::Vector3 & origin)
	: TGen::SceneNode(name, origin)
{
}

void TGen::Engine::Map::addModel(TGen::Engine::MapModel * model) {
	if (models.find(model->getName()) != models.end())
		throw TGen::RuntimeException("Map::addModel", "model '" + model->getName() + "' already exists");
	
	models.insert(ModelMap::value_type(model->getName(), model));
}

bool TGen::Engine::Map::fillFaces(TGen::RenderList & list, const TGen::Camera & camera) const {
	//for (ModelMap::const_iterator iter = models.begin(); iter != models.end(); ++iter)
	//	iter->second->fillFaces(list, camera);
	
	return true;
}

bool TGen::Engine::Map::fillUser(TGen::RenderList & list, const TGen::Camera & camera) const {
	// lights etc
	
	return true;	
}

void TGen::Engine::Map::linkMaterial(TGen::MaterialSource & source) {
	for (ModelMap::iterator iter = models.begin(); iter != models.end(); ++iter)
		iter->second->linkMaterial(source);
}

void TGen::Engine::Map::unlinkMaterial() {
	for (ModelMap::iterator iter = models.begin(); iter != models.end(); ++iter)
		iter->second->unlinkMaterial();
}

void TGen::Engine::Map::instantiate(TGen::VertexDataSource & source) {
	for (ModelMap::iterator iter = models.begin(); iter != models.end(); ++iter)
		iter->second->instantiate(source);
}
