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
	, rootNode(NULL)
{
}

TGen::Engine::Map::~Map() {
	nodes.clear();
	delete rootNode;
}

void TGen::Engine::Map::addModel(TGen::Engine::MapModel * model) {
	if (models.find(model->getName()) != models.end())
		throw TGen::RuntimeException("Map::addModel", "model '" + model->getName() + "' already exists");
	
	models.insert(ModelMap::value_type(model->getName(), model));
}

bool TGen::Engine::Map::fillFaces(TGen::RenderList & list, const TGen::Camera & camera) const {	
	TGen::Matrix4x4 invertedCam = camera.getTransform();
	invertedCam.invert();

	TGen::Vector3 pos = getTransform().getInverse() * invertedCam.getOrigin();
	//pos.y = -pos.y;
	//std::cout << std::string(pos) << std::endl;
	//std::cout << std::string(getTransform().getOrigin()) << std::endl;
	
	TGen::Engine::MapLeafNode * leafNode = const_cast<TGen::Engine::MapLeafNode *>(getLeafNode(pos));
	//std::cout << "done" << std::endl;
	
	if (!leafNode) {
		for (ModelMap::const_iterator iter = models.begin(); iter != models.end(); ++iter)
			iter->second->fillFaces(list, iter->second->getOverridingMaterial(), this);
	}
	else {
		//std::cout << "IN LEAF" << std::endl;
		
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

void TGen::Engine::Map::addNode(TGen::Engine::MapLinkNode * node) {
	nodes.push_back(node);
}

void TGen::Engine::Map::setRootNode(TGen::Engine::MapLinkNode * root) {
	delete rootNode;
	
	rootNode = root;
}

TGen::Engine::MapLinkNode * TGen::Engine::Map::getNode(int num) {
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
	TGen::Engine::MapLinkNode * node = rootNode;
	while (node) {
		scalar side = node->plane.getPointSide(TGen::Vector3(-position.x, -position.y, -position.z));
		//std::cout << node << std::endl;
		
		TGen::Engine::MapLeafNode * leaf = NULL;
		
	//	std::cout << "SIDE: " << side << std::endl;
		
		if (side <= 0) {
			leaf = node->posLeaf;
			node = node->pos;
			
		//	if (!node)
		//		std::cout << "no pos node" << std::endl;
		}
		else {
			leaf = node->negLeaf;
			node = node->neg;

			//if (!node)
		//		std::cout << "no neg node" << std::endl;
		}
		
		if (leaf) {
		//	std::cout << "found leaf" << std::endl;
			return leaf;
		}		
	}
	
	
	return NULL;
}

bool TGen::Engine::Map::fillMeta(TGen::RenderList & list, const TGen::Camera & camera) const {
	for (ModelMap::const_iterator iter = models.begin(); iter != models.end(); ++iter) {
		list.addMeta(iter->second, this);
	}
	
	return true;
}


