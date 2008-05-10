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
#include "mapportal.h"

TGen::Engine::Map::Map(const std::string & name, const TGen::Vector3 & origin)
	: TGen::SceneNode(name, origin)
	, rootNode(NULL)
{
}

TGen::Engine::Map::~Map() {
	//nodes.clear();
	delete rootNode;
	
	for (PortalList::iterator iter = portals.begin(); iter != portals.end(); ++iter)
		delete *iter;
	
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

	
	TGen::Engine::MapLeafNode * leafNode = const_cast<TGen::Engine::MapLeafNode *>(getLeafNode(pos));
	//std::cout << "done" << std::endl;
	
	if (!leafNode) {
		for (ModelMap::const_iterator iter = models.begin(); iter != models.end(); ++iter)
			iter->second->fillFaces(list, iter->second->getOverridingMaterial(), this);
	}
	else {
		TGen::Engine::MapModel * leaf = leafNode->getModel();
		modelRendered.clear();
		
		fillModels(leaf, list, camera);
		
		std::cout << "areas rendered: " << modelRendered.size() << std::endl;
	}
		
	return true;
}

void TGen::Engine::Map::fillModels(TGen::Engine::MapModel * leaf, TGen::RenderList & list, const TGen::Camera & camera) const {
	modelRendered[leaf] = true;
	
	
	TGen::Matrix4x4 camOffset = camera.getTransform();
	camOffset.invert();
	camOffset *= TGen::Matrix4x4::Translation(TGen::Vector3(0.0f, 0.0f, 2.0f));
	camOffset.invert();
	
	for (int i = 0; i < leaf->getNumPortals(); ++i) {
		TGen::Engine::MapPortal * portal = leaf->getPortal(i);
		portal->open = false;
		
		TGen::Vector2 min, max;
		TGen::Vector2 min2, max2;
		
		bool first = true;
		
		for (int i = 0; i < portal->getNumPoints(); ++i) {
			TGen::Vector4 point = portal->getPoint(i);
			TGen::Vector4 devcoord = camera.getProjection() * camera.getTransform() * getTransform() * point;
			devcoord /= devcoord.w;

			TGen::Vector2 vect(devcoord.x, devcoord.y);
			
			TGen::Vector4 devcoord2 = camera.getProjection() * camOffset * getTransform() * point;
			devcoord2 /= devcoord2.w;
			
			if ((devcoord.z < -1.0f || devcoord.z > 1.0f) && (devcoord2.z < -1.0f || devcoord2.z > 1.0f))
				continue;
			
			TGen::Vector2 vect2(devcoord2.x, devcoord2.y);
			
			//std::cout << std::string(TGen::Vector3(devcoord)) << std::endl;
			if (first) {
				min = max = vect;
				min2 = max2 = vect2;
				first = false;
			}
			else {
				if (vect.x < min.x)
					min.x = vect.x;
				if (vect.y < min.y)
					min.y = vect.y;
				
				if (vect.x > max.x)
					max.x = vect.x;
				if (vect.y > max.y)
					max.y = vect.y;
				
				if (vect2.x < min2.x)
					min2.x = vect2.x;
				if (vect2.y < min2.y)
					min2.y = vect2.y;
				
				if (vect2.x > max2.x)
					max2.x = vect2.x;
				if (vect2.y > max2.y)
					max2.y = vect2.y;
			}
			
			TGen::Rectangle portalRect(min, max);
			TGen::Rectangle portalRect2(min2, max2);

			TGen::Rectangle screenRect(TGen::Vector2(-1.0f, -1.0f), TGen::Vector2(1.0f, 1.0f));
			
			if (portalRect.intersects(screenRect) || portalRect2.intersects(screenRect))
				portal->open = true;
		}
		
		if (portal->open) {
			TGen::Matrix4x4 invertedCam = camera.getTransform();
			invertedCam.invert();
			
			TGen::Vector3 pos = getTransform().getInverse() * invertedCam.getOrigin();

			
			if (portal->getPlane().getDistanceTo(pos) < 0.0) {
				if (!modelRendered[portal->getNegArea()])
					fillModels(portal->getNegArea(), list, camera);
			}
			else {
				if (!modelRendered[portal->getPosArea()])
					fillModels(portal->getPosArea(), list, camera);
			}
		}
	}
	
	TGen::Material * mat = leaf->getOverridingMaterial();
	leaf->fillFaces(list, leaf->getOverridingMaterial(), this);	
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
	// TODO: check if we're already instantiated
	
	for (ModelMap::iterator iter = models.begin(); iter != models.end(); ++iter)
		iter->second->instantiate(source);
	
	for (PortalList::iterator iter = portals.begin(); iter != portals.end(); ++iter) {
		(*iter)->link(this);
		
		if ((*iter)->getPosArea())
			(*iter)->getPosArea()->addPortal(*iter);
		if ((*iter)->getNegArea())
			(*iter)->getNegArea()->addPortal(*iter);		
	}
	
}

void TGen::Engine::Map::addNode(TGen::Engine::MapLinkNode * node) {
	nodes.push_back(node);
}

void TGen::Engine::Map::setRootNode(TGen::Engine::MapLinkNode * root) {
	delete rootNode;
	
	rootNode = root;
}

TGen::Engine::MapLinkNode * TGen::Engine::Map::getNode(int num) {
	std::cout << "request for node " << num << std::endl;
	return nodes.at(num);
}

TGen::Engine::MapModel * TGen::Engine::Map::getModel(const std::string & name) {
	ModelMap::iterator iter = models.find(name);
	if (iter == models.end()) {
		std::cout << "NOT FOUND AREA " + name << std::endl;
		throw TGen::RuntimeException("Map::getModel", "model not found: '" + name + "'");
	}
	
	std::cout << "FOUND AREA " + name + " at address " << iter->second << std::endl;
	
	return iter->second;
}

TGen::Engine::MapLeafNode * TGen::Engine::Map::getLeafNode(const TGen::Vector3 & position) const {
	//std::cout << std::string(position) << std::endl;
	TGen::Engine::MapLinkNode * node = rootNode;
	while (node) {
		scalar side = node->plane.getPointSide(TGen::Vector3(-position.x, -position.y, -position.z));
		
		TGen::Engine::MapLeafNode * leaf = NULL;
		
		//std::cout << "SIDE: " << side << std::endl;
		
		if (side <= 0) {
			leaf = node->posLeaf;
			node = node->pos;
			
			//if (!node)
			//	std::cout << "no pos node" << std::endl;
		}
		else {
			leaf = node->negLeaf;
			node = node->neg;

			//if (!node)
			//	std::cout << "no neg node" << std::endl;
		}
		
		if (leaf) {
			//std::cout << "found leaf " << leaf << std::endl;
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

void TGen::Engine::Map::addPortal(TGen::Engine::MapPortal * portal) {
	portals.push_back(portal);
}

