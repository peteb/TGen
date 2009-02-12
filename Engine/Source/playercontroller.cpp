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
#include "componentlinker.h"
#include <tgen_renderer.h>

TGen::Engine::PlayerController::PlayerController(const std::string & name) 
	: TGen::Engine::Component(name)
	, ignoreInput(false)
{
	for (int i = 0; i < 20; ++i)
		activeEvents[i] = 0;	
}

TGen::Engine::PlayerController::~PlayerController() {
	
}

void TGen::Engine::PlayerController::beginEvent(int id) {
	if (id >= 0 && id < 20 && !ignoreInput)
		activeEvents[id] |= EventTriggered;
}

void TGen::Engine::PlayerController::endEvent(int id) {
	if (id >= 0 && id < 20 && !ignoreInput) {
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

void TGen::Engine::PlayerController::setEventRead(int id) {
	if (id < 0 || id >= 20)
		return;
	
	activeEvents[id] |= EventRead;
}

bool TGen::Engine::PlayerController::isEventInitial(int id) const {
	if (id < 0 || id > 20)
		return false;
	
	return activeEvents[id] & EventTriggered && !(activeEvents[id] & EventRead);	
}

TGen::Vector3 TGen::Engine::PlayerController::checkViewDelta() {
	if (ignoreInput) {
		return TGen::Vector3::Zero;
	}
	
	TGen::Vector3 ret = viewDelta;
	viewDelta = TGen::Vector3::Zero;
	
	return ret;
}

void TGen::Engine::PlayerController::link(const TGen::Engine::ComponentLinker & linker) {
	camera.link(linker);
}

void TGen::Engine::PlayerController::addViewDelta(const TGen::Vector3 & view) {
	viewDelta += view;
	viewAbs = view;
}

TGen::Vector3 TGen::Engine::PlayerController::checkViewAbs() {
	return viewAbs;
}

TGen::Vector3 TGen::Engine::PlayerController::getVelocity() const {
	return TGen::Vector3::Zero;
}

TGen::Vector3 TGen::Engine::PlayerController::getPosition() const {
	return TGen::Vector3::Zero;
}

TGen::Rotation TGen::Engine::PlayerController::getOrientation() const {
	return TGen::Rotation::Identity;	
}

void TGen::Engine::PlayerController::resetEvents() {
	for (int i = 0; i < 20; ++i) 
		activeEvents[i] = 0;
}

TGen::Engine::Scene::Node * TGen::Engine::PlayerController::getActiveCamera() const {
	return camera.get();
}

void TGen::Engine::PlayerController::setCamera(const std::string & name) {
	camera.set(name);
}
