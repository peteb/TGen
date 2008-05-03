/*
 *  body.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 2/29/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "physics/body.h"
#include "scene/node.h"
#include "entity.h"
#include <tgen_renderer.h>

// TODO: sceneMap -> sceneNode portalMap i entities

TGen::Engine::Physics::Body::Body(const std::string & name, dBodyID bodyId) 
	: TGen::Engine::Component(name)
	, bodyId(bodyId)
	, sceneNodeComponent(NULL)
	, turnHeadwise(false)
	, killTorque(false)
{

}

TGen::Engine::Physics::Body::~Body() {
	dBodyDestroy(bodyId);
}

void TGen::Engine::Physics::Body::preStep() {
	if (sceneNodeComponent) {
		setPosition(sceneNodeComponent->getSceneNode()->getLocalPosition());
		setOrientation(sceneNodeComponent->getSceneNode()->getLocalOrientation());
	}
	
	if (killTorque)
		dBodySetTorque(bodyId, 0.0, 0.0, 0.0);
}

void TGen::Engine::Physics::Body::postStep() {
	if (sceneNodeComponent) {
		sceneNodeComponent->getSceneNode()->setPosition(getPosition());
		
		if (turnHeadwise)
			sceneNodeComponent->getSceneNode()->setOrientation(getOrientation() * TGen::Rotation::RotationX(TGen::Radian(TGen::HALF_PI)));			
		else
			sceneNodeComponent->getSceneNode()->setOrientation(getOrientation());
	}
}

void TGen::Engine::Physics::Body::linkLocally(TGen::Engine::Entity & entity) {
	sceneNodeComponent = dynamic_cast<TGen::Engine::Scene::Node *>(entity.getComponent(nodeComponent));
	
	setPosition(sceneNodeComponent->getSceneNode()->getLocalPosition());
}

void TGen::Engine::Physics::Body::setPosition(const TGen::Vector3 & position) {
	dBodySetPosition(bodyId, position.x, position.y, position.z);
}

TGen::Vector3 TGen::Engine::Physics::Body::getPosition() const {
	const dReal * vec = dBodyGetPosition(bodyId);
	
	return TGen::Vector3(vec[0], vec[1], vec[2]);
}

dBodyID TGen::Engine::Physics::Body::getBodyId() const {
	return bodyId;
}

TGen::Matrix3x3 TGen::Engine::Physics::Body::getOrientation() const {
	const dReal * orient = dBodyGetRotation(bodyId);
	
	// TODO: transpose this if something is wrong
	TGen::Vector3 x(orient[0], orient[4], orient[8]);
	TGen::Vector3 y(orient[1], orient[5], orient[9]);
	TGen::Vector3 z(orient[2], orient[6], orient[10]);
	
	return TGen::Matrix3x3(x, y, z);
}

void TGen::Engine::Physics::Body::setOrientation(const TGen::Matrix3x3 & orientation) {
	dMatrix3 matrix;
	TGen::Vector3 x = orientation.getX();
	TGen::Vector3 y = orientation.getY();
	TGen::Vector3 z = orientation.getZ();
	
	// TODO: and transpose here...
	matrix[0] = x.x;
	matrix[1] = y.x;
	matrix[2] = z.x;
	matrix[3] = 0.0f;
	matrix[4] = x.y;
	matrix[5] = y.y;
	matrix[6] = z.y;
	matrix[7] = 0.0f;
	matrix[8] = x.z;
	matrix[9] = y.z;
	matrix[10] = z.z;
	matrix[11] = 0.0f;
	
	dBodySetRotation(bodyId, matrix);
}

void TGen::Engine::Physics::Body::setTurnHeadwise(bool turnHeadwise) {
	this->turnHeadwise = turnHeadwise;
}

void TGen::Engine::Physics::Body::setMaxAngularSpeed(scalar speed) {
	if (speed > -0.5f)
		dBodySetMaxAngularSpeed(bodyId, speed);
}

void TGen::Engine::Physics::Body::setNodeComponent(const std::string & nodeName) {
	nodeComponent = nodeName;
}

void TGen::Engine::Physics::Body::setKillTorque(bool killTorque) {
	this->killTorque = killTorque;
}

void TGen::Engine::Physics::Body::addForce(const TGen::Vector3 & force) {
	dBodyAddForce(bodyId, force.x, force.y, force.z);
}

TGen::Vector3 TGen::Engine::Physics::Body::getLinearVelocity() const {
	const dReal * force = dBodyGetLinearVel(bodyId);
	return TGen::Vector3(force[0], force[1], force[2]);
}

void TGen::Engine::Physics::Body::setLinearDamping(scalar damping) {
	dBodySetLinearDamping(bodyId, damping);
}

