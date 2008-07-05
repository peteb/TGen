/*
 *  arcball.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 4/27/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include <tgen_renderer.h>
#include "controller/arcball.h"
#include "scene/node.h"
#include "entity.h"
#include "gameinputmapper.h"

TGen::Engine::Controller::Arcball::Arcball(const std::string & name) 
	: TGen::Engine::PlayerController(name)
	, node(NULL)
	, thisRot(TGen::Rotation::Identity)
	, lastRot(TGen::Rotation::Identity)
	, camera(NULL)
{
	
}


TGen::Engine::Controller::Arcball::~Arcball() {
	
}


void TGen::Engine::Controller::Arcball::linkLocally(TGen::Engine::Entity & entity) {
	TGen::Engine::PlayerController::linkLocally(entity);

	TGen::Engine::Scene::Node & sceneNode = dynamic_cast<TGen::Engine::Scene::Node &>(entity.getComponent(controlName));
	
	node = sceneNode.getSceneNode();
	camera = getCamera("headcam");
}


void TGen::Engine::Controller::Arcball::update(scalar dt) {
	if (!node)
		return;
	
	if (isEventInitial(TGen::Engine::EventPrimaryFire)) {		// start
		setEventRead(TGen::Engine::EventPrimaryFire);
		start = mapToSphere(checkViewAbs());
		lastRot *= thisRot;
	}
	else if (checkEvent(TGen::Engine::EventPrimaryFire)) {	// drag
		thisRot = dragTo(mapToSphere(checkViewAbs()));
		
		TGen::Rotation totalRot = lastRot * thisRot;
		node->setOrientation(totalRot);
	}

	if (camera) {
		if (checkEvent(TGen::Engine::EventForward))
			camera->setPosition(camera->getLocalPosition() + TGen::Vector3(0.0f, 0.0f, -1.0f) * dt);

		if (checkEvent(TGen::Engine::EventBackward))
			camera->setPosition(camera->getLocalPosition() + TGen::Vector3(0.0f, 0.0f, 1.0f) * dt);

		if (checkEvent(TGen::Engine::EventStrafeLeft))
			camera->setPosition(camera->getLocalPosition() + TGen::Vector3(-1.0f, 0.0f, 0.0f) * dt);
		
		if (checkEvent(TGen::Engine::EventStrafeRight))
			camera->setPosition(camera->getLocalPosition() + TGen::Vector3(1.0f, 0.0f, 0.0f) * dt);
	
		if (checkEvent(TGen::Engine::EventJump))
			camera->setPosition(camera->getLocalPosition() + TGen::Vector3(0.0f, 1.0f, 0.0f) * dt);

		if (checkEvent(TGen::Engine::EventCrouch))
			camera->setPosition(camera->getLocalPosition() + TGen::Vector3(0.0f, -1.0f, 0.0f) * dt);
			
	}
	
}


bool TGen::Engine::Controller::Arcball::useRelativeView() const {
	return false;
}


void TGen::Engine::Controller::Arcball::setControl(const std::string & controlName) {
	this->controlName = controlName;
}


TGen::Vector3 TGen::Engine::Controller::Arcball::mapToSphere(const TGen::Vector3 & vec) const {
	TGen::Vector3 inPos = vec;
	
	inPos = (inPos / 256.0f) - 1.0f;
	inPos.y = -inPos.y;
	
	TGen::Vector3 newPoint;
	
	if (inPos.getMagnitude() > 1.0f) {
		newPoint = inPos.getNormalized();
		newPoint.z = 1.0f;
	}
	else {
		newPoint = inPos;
		newPoint.z = sqrt(1.0f - inPos.getMagnitude());
	}
	
	return newPoint;
}


TGen::Rotation TGen::Engine::Controller::Arcball::dragTo(const TGen::Vector3 & end) {
	TGen::Vector3 perp = TGen::Vector3::CrossProduct(start, end);
	TGen::Quaternion4 rotation;
	rotation.x = perp.x;
	rotation.y = perp.y;
	rotation.z = perp.z;
	rotation.w = TGen::Vector3::DotProduct(start, end);
	
	return TGen::Rotation(rotation);
}
