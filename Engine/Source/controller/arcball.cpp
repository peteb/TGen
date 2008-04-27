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

TGen::Engine::Controller::Arcball::Arcball(const std::string & name, const std::string & nodeName) 
	: TGen::Engine::PlayerController(name)
	, node(NULL)
	, nodeName(nodeName)
	, thisRot(TGen::Rotation::Identity)
	, lastRot(TGen::Rotation::Identity)
{
	
}

TGen::Engine::Controller::Arcball::~Arcball() {
	
}


void TGen::Engine::Controller::Arcball::linkLocally(TGen::Engine::Entity & entity) {
	TGen::Engine::PlayerController::linkLocally(entity);

	TGen::Engine::Scene::Node * sceneNode = dynamic_cast<TGen::Engine::Scene::Node *>(entity.getComponent(nodeName));
	if (!sceneNode)
		throw TGen::RuntimeException("Controller::Arcball::linkLocally", "no node found called '" + nodeName + "'");
	
	node = sceneNode->getSceneNode();
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
}

bool TGen::Engine::Controller::Arcball::useRelativeView() const {
	return false;
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
