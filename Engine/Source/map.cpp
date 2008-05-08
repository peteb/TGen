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
#include "mapnode.h"

TGen::Engine::Map::Map(const std::string & name, const TGen::Vector3 & origin)
	: TGen::SceneNode(name, origin)
	, nodeRoot(NULL)
{
}

TGen::Engine::Map::~Map() {
	nodes.clear();
	delete nodeRoot;
}

void TGen::Engine::Map::addModel(TGen::Engine::MapModel * model) {
	if (models.find(model->getName()) != models.end())
		throw TGen::RuntimeException("Map::addModel", "model '" + model->getName() + "' already exists");
	
	models.insert(ModelMap::value_type(model->getName(), model));
}

bool TGen::Engine::Map::fillFaces(TGen::RenderList & list, const TGen::Camera & camera) const {	
	TGen::Engine::MapLeafNode * leafNode = const_cast<TGen::Engine::MapLeafNode *>(getLeafNode((getTransform() * camera.getTransform()).getOrigin()));
	
	if (!leafNode) {
		for (ModelMap::const_iterator iter = models.begin(); iter != models.end(); ++iter)
			iter->second->fillFaces(list, iter->second->getOverridingMaterial(), this);
	}
	else {
		leafNode->getModel()->fillFaces(list, leafNode->getModel()->getOverridingMaterial(), this);
	}
		
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

void TGen::Engine::Map::addNode(TGen::Engine::MapNode * node) {
	nodes.push_back(node);
}

void TGen::Engine::Map::setNodeRoot(TGen::Engine::MapNode * root) {
	if (nodeRoot)
		delete nodeRoot;
	
	nodeRoot = root;
}

TGen::Engine::MapNode * TGen::Engine::Map::getNode(int num) {
	return nodes.at(num);
}

TGen::Engine::MapModel * TGen::Engine::Map::getModel(const std::string & name) {
	ModelMap::iterator iter = models.find(name);
	if (iter == models.end())
		throw TGen::RuntimeException("Map::getModel", "model not found: '" + name + "'");
		
	return iter->second;
}

TGen::Engine::MapLeafNode * TGen::Engine::Map::getLeafNode(const TGen::Vector3 & position) const {
	//std::cout << std::string(position) << std::endl;
	TGen::Engine::MapLinkNode * node = nodeRoot;
	while (node) {
		scalar side = node->plane.getPointSize(position);
		
		// TODO: en LinkNode kan ha pekare till leaves också, posLeaf, negLeaf, istället för polymorf
		
		if (side > 0) {
			
		}
		else {
			
		}
		
	}
	
	return NULL;
}


