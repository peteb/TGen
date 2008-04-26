/*
 *  playercontroller.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 4/11/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "playercontroller.h"
#include "scene/node.h"
#include "entity.h"
#include <tgen_renderer.h>

TGen::Engine::PlayerController::PlayerController(const std::string & name) 
	: TGen::Engine::Component(name)
{
	for (int i = 0; i < 20; ++i)
		activeEvents[i] = 0;	
}

TGen::Engine::PlayerController::~PlayerController() {
	
}

void TGen::Engine::PlayerController::beginEvent(int id) {
	if (id >= 0 && id < 20)
		activeEvents[id] |= EventTriggered;
}

void TGen::Engine::PlayerController::endEvent(int id) {
	if (id >= 0 && id < 20) {
		if (!(activeEvents[id] & EventRead))
			activeEvents[id] |= EventKilled;
		else
			activeEvents[id] = 0;
	}
}

bool TGen::Engine::PlayerController::checkEvent(int id) {
	if (id < 0 || id >= 20)
		return false;
	
	if (activeEvents[id] & EventTriggered) {
		if (activeEvents[id] & EventKilled)
			activeEvents[id] = 0;
		else if (!(activeEvents[id] & EventRead))
			activeEvents[id] |= EventRead;
		
		return true;
	}
	
	return false;
}

TGen::Vector3 TGen::Engine::PlayerController::checkViewDelta() {
	TGen::Vector3 ret = viewDelta;
	viewDelta = TGen::Vector3::Zero;
	
	return ret;
}

// TODO: fix blog entry
// TODO: mouse steering, that ball thingie (arcball?)

/*void TGen::Engine::PlayerController::addCamera(const std::string & name, TGen::Camera * camera) {
	cameras.insert(CameraMap::value_type(name, camera));
}*/

TGen::Camera * TGen::Engine::PlayerController::getCamera(const std::string & name) const {
	CameraMap::const_iterator iter = cameras.find(name);
	if (iter == cameras.end())
		throw TGen::RuntimeException("PlayerController::getCamera", "camera '" + name + "' not found");
	
	return iter->second;
}


void TGen::Engine::PlayerController::addCamera(const std::string & name, const std::string & camera) {
	camerasToLink.insert(StringStringMap::value_type(name, camera));
}


void TGen::Engine::PlayerController::linkCameras(TGen::Engine::Entity & entity) {
	cameras.clear();
	
	for (StringStringMap::iterator iter = camerasToLink.begin(); iter != camerasToLink.end(); ++iter) {
		TGen::Engine::Scene::Node * camNode = dynamic_cast<TGen::Engine::Scene::Node *>(entity.getComponent(iter->second));
		cameras.insert(CameraMap::value_type(iter->first, dynamic_cast<TGen::Camera *>(camNode->getSceneNode())));
	}
}

void TGen::Engine::PlayerController::linkLocally(TGen::Engine::Entity & entity) {
	linkCameras(entity);
}

void TGen::Engine::PlayerController::addViewDelta(const TGen::Vector3 & view) {
	viewDelta += view;
}
