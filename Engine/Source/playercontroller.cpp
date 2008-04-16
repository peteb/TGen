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

TGen::Engine::PlayerController::PlayerController(const std::string & name, scalar speed) 
	: TGen::Engine::Component(name)
	, speed(speed)
	, node(NULL)
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

void TGen::Engine::PlayerController::update(scalar dt) {
	
	if (checkEvent(EventForward)) {
		if (node) {
			node->setPosition(node->getLocalPosition() - node->getLocalOrientation().getZ() * dt);
		}
	}
	if (checkEvent(EventBackward)) {
		if (node) {
			node->setPosition(node->getLocalPosition() + node->getLocalOrientation().getZ() * dt);
		}
	}
	if (checkEvent(EventStrafeLeft)) {
		if (node) {
			node->setPosition(node->getLocalPosition() - node->getLocalOrientation().getX() * dt);
		}
	}
	if (checkEvent(EventStrafeRight)) {
		if (node) {
			node->setPosition(node->getLocalPosition() + node->getLocalOrientation().getX() * dt);
		}
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

/*void TGen::Engine::PlayerController::addCamera(const std::string & name, TGen::Camera * camera) {
	cameras.insert(CameraMap::value_type(name, camera));
}*/

TGen::Camera * TGen::Engine::PlayerController::getCamera(const std::string & name) const {
	CameraMap::const_iterator iter = cameras.find(name);
	if (iter == cameras.end())
		throw TGen::RuntimeException("PlayerController::getCamera", "camera '" + name + "' not found");
	
	return iter->second;
}

void TGen::Engine::PlayerController::linkLocally(TGen::Engine::Entity & entity) {
	TGen::Engine::Scene::Node * playNode = dynamic_cast<TGen::Engine::Scene::Node *>(entity.getComponent("sceneNode"));
	// kanske pga arvet?
	
	// såhär: om man inte anger namn på en component så heter den samma som typen, dvs sceneNode. Det heter den i entitetens komponentlista
	//        men i subsystemet kan den heta något annat..... förmodligen entitetens namn
	
	// TODO: subclassa, GhostController
	// TODO: använd värdena camera och control i controller-component
	
	this->node = dynamic_cast<TGen::SceneNode *>(playNode->getSceneNode());

	//TGen::Camera * cam = dynamic_cast<TGen::Camera *>(camNode->getSceneNode());
	
	linkCameras(entity);
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
