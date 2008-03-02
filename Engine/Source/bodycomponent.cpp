/*
 *  bodycomponent.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 2/29/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "bodycomponent.h"
#include "scenenodecomponent.h"
#include "entity.h"
#include <tgen_renderer.h>

// TODO: sceneMap -> sceneNode portalMap i entities

TGen::Engine::BodyComponent::BodyComponent(const std::string & name, dBodyID bodyId) 
	: TGen::Engine::Component(name)
	, bodyId(bodyId)
	, sceneNodeComponent(NULL)
{
	
}

TGen::Engine::BodyComponent::~BodyComponent() {
	dBodyDestroy(bodyId);
}

void TGen::Engine::BodyComponent::preStep() {
	if (sceneNodeComponent)
		setPosition(sceneNodeComponent->getSceneNode()->getLocalPosition());
}

void TGen::Engine::BodyComponent::postStep() {
	//position -= TGen::Vector3(0.0f, 1.0f, 0.0f) * 1.0f * dt;
	
	if (sceneNodeComponent) {
		sceneNodeComponent->getSceneNode()->setPosition(getPosition());
		sceneNodeComponent->getSceneNode()->setOrientation((getOrientation().getZ()).normalize());
	}
}

void TGen::Engine::BodyComponent::linkLocally(TGen::Engine::Entity & entity) {
	sceneNodeComponent = dynamic_cast<TGen::Engine::SceneNodeComponent *>(entity.getComponent("sceneNode"));
	
	
	setPosition(sceneNodeComponent->getSceneNode()->getLocalPosition());
}

void TGen::Engine::BodyComponent::setPosition(const TGen::Vector3 & position) {
	dBodySetPosition(bodyId, position.x, position.y, position.z);
}

TGen::Vector3 TGen::Engine::BodyComponent::getPosition() const {
	const dReal * vec = dBodyGetPosition(bodyId);
	
	return TGen::Vector3(vec[0], vec[1], vec[2]);
}

dBodyID TGen::Engine::BodyComponent::getBodyId() const {
	return bodyId;
}

TGen::Matrix3x3 TGen::Engine::BodyComponent::getOrientation() const {
	const dReal * orient = dBodyGetRotation(bodyId);
	
	//return TGen::Matrix3x3(orient[0+3*0], orient[0+3*1], orient[0+3*2], orient[1+3*0], orient[1+3*1], orient[1+3*2], orient[2+3*0], orient[2+3*1], orient[2+4*2]);	
	
	return TGen::Matrix3x3(TGen::Vector3(orient[0], orient[1], orient[2]),
								  TGen::Vector3(orient[4], orient[5], orient[6]),
								  TGen::Vector3(orient[8], orient[9], orient[10]));
}

void TGen::Engine::BodyComponent::setOrientation(const TGen::Matrix3x3 & orientation) {
	dMatrix3 matrix;
	matrix[0] = orientation(0, 0);
	matrix[1] = orientation(1, 0);
	matrix[2] = orientation(2, 0);
	matrix[3] = 0.0f;
	matrix[4] = orientation(0, 1);
	matrix[5] = orientation(1, 1);
	matrix[6] = orientation(2, 1);
	matrix[7] = 0.0f;
	matrix[8] = orientation(0, 2);
	matrix[9] = orientation(1, 2);
	matrix[10] = orientation(2, 2);
	matrix[11] = 0.0f;
	
}
