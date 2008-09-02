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

using TGen::scalar;

#define BODY TGen::Engine::Physics::Body

TGen::Engine::Symbol BODY::symbolSetUpdateFromScene = TGen::Engine::getUniqueSymbol("setUpdateFromScene:");
TGen::Engine::Symbol BODY::symbolSetMaxAngularSpeed = TGen::Engine::getUniqueSymbol("setMaxAngularSpeed:");
TGen::Engine::Symbol BODY::symbolSetKillTorque = TGen::Engine::getUniqueSymbol("setKillTorque:");
TGen::Engine::Symbol BODY::symbolTransportTo = TGen::Engine::getUniqueSymbol("transportTo:");
TGen::Engine::Symbol BODY::symbolResetForces = TGen::Engine::getUniqueSymbol("resetForces");
TGen::Engine::Symbol BODY::symbolAddForce = TGen::Engine::getUniqueSymbol("addForceX:Y:Z:");


TGen::Engine::Physics::Body::Body(const std::string & name, dBodyID bodyId, dWorldID worldId, dSpaceID spaceId) 
	: TGen::Engine::Component(name)
	, bodyId(bodyId)
	, turnHeadwise(false)
	, killTorque(false)
	, onFloor(false)
	, fakeGrav(-4.0f)
	, worldId(worldId)
	, spaceId(spaceId)
	, doUpdateFromScene(true)
{
}


TGen::Engine::Physics::Body::~Body() {
	dBodyDestroy(bodyId);
}


void TGen::Engine::Physics::Body::preStep() {
	if (fakeGrav > -2.0 && !onFloor) {
		TGen::Vector3 realGrav(0.0f, -20.0f, 0.0f);
		realGrav *= fakeGrav;
		
		dBodyAddForce(bodyId, realGrav.x, realGrav.y, realGrav.z);
	}
	
	onFloor = false;
	
	updateFromScene();
	
	if (killTorque)
		dBodySetTorque(bodyId, 0.0, 0.0, 0.0);
}


void TGen::Engine::Physics::Body::postStep() {
	updateScene();
	
	TGen::Vector3 thisPos = getPosition();
	TGen::Vector3 diff = thisPos - lastPosition;
	lastPosition = thisPos;
	linearVelocity = thisPos / 60.0;
	
}


void TGen::Engine::Physics::Body::trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode) {
	TGen::Engine::Symbol symbolNum = context.getFunctionSymbol();
	
	if (symbolNum == symbolSetUpdateFromScene) {
		int updateFromScene = context.getRegister<int>(2);
		this->doUpdateFromScene = updateFromScene;
	}
	else if (symbolNum == symbolSetMaxAngularSpeed) {
		scalar maxAngularSpeed = context.getRegister<scalar>(2);
		
		if (maxAngularSpeed < 0.0)
			setMaxAngularSpeed(dInfinity);
		else
			setMaxAngularSpeed(maxAngularSpeed);
	}
	else if (symbolNum == symbolSetKillTorque) {
		int killTorque = context.getRegister<int>(2);
		
		setKillTorque(killTorque);
	}
	else if (symbolNum == symbolTransportTo) {
		TGen::Engine::Component * transportTo = context.getParameterPtr<TGen::Engine::Component *>(0);
		TGen::Engine::WorldObject * worldObject = dynamic_cast<TGen::Engine::WorldObject *>(transportTo);
		
		setPosition(worldObject->getPosition());
		std::cout << "BODY TRANSPORT TO " << std::string(worldObject->getPosition()) << std::endl;

		updateScene();
		
		//setOrientation(worldObject->getOrientation());
	}
	else if (symbolNum == symbolAddForce) {
		TGen::Vector3 force(context.getParameter<scalar>(0), context.getParameter<scalar>(1), context.getParameter<scalar>(2));
		addForce(force);
	}
	else if (symbolNum == symbolResetForces) {
		resetForces();
	}
	else {
		TGen::Engine::Component::trigger(context, mode);
	}
}


void TGen::Engine::Physics::Body::link(const TGen::Engine::ComponentLinker & linker) {
	delegate.link(linker);
	updateFromScene();
}


void TGen::Engine::Physics::Body::setPosition(const TGen::Vector3 & position) {
	dBodySetPosition(bodyId, position.x, position.y, position.z);
}


TGen::Vector3 TGen::Engine::Physics::Body::getPosition() const {
	const dReal * vec = dBodyGetPosition(bodyId);
	
	return TGen::Vector3(vec[0], vec[1], vec[2]);
}


dWorldID TGen::Engine::Physics::Body::getWorldId() {
	return worldId;
}


dSpaceID TGen::Engine::Physics::Body::getSpaceId() {
	return spaceId;
}


dBodyID TGen::Engine::Physics::Body::getBodyId() const {
	return bodyId;
}


void TGen::Engine::Physics::Body::updateFromScene() {	
	if (delegate && doUpdateFromScene) {
		setPosition(delegate->getPosition());
		setOrientation(delegate->getOrientation());
	}	
}


void TGen::Engine::Physics::Body::updateScene() {
	if (delegate) {
		delegate->setPosition(getPosition());
		
		if (turnHeadwise)
			delegate->setOrientation(getOrientation() * TGen::Rotation::RotationX(TGen::Radian(TGen::HALF_PI)));			
		else
			delegate->setOrientation(getOrientation());
	}
}


TGen::Rotation TGen::Engine::Physics::Body::getOrientation() const {
	const dReal * orient = dBodyGetRotation(bodyId);
	
	// TODO: transpose this if something is wrong
	TGen::Vector3 x(orient[0], orient[4], orient[8]);
	TGen::Vector3 y(orient[1], orient[5], orient[9]);
	TGen::Vector3 z(orient[2], orient[6], orient[10]);
	
	return TGen::Rotation(x, y, z);
}

// TODO: varför är orientation fel? den sätts dock EFTER att body har blivit länkad, så den får inte orientation initialt, men borde ju ändå få när man kör preStep?

void TGen::Engine::Physics::Body::setOrientation(const TGen::Rotation & orientation) {
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


void TGen::Engine::Physics::Body::setLink(const std::string & linkName) {
	delegate.set(linkName);
}


void TGen::Engine::Physics::Body::setLink(TGen::Engine::WorldObject * linkedTo) {
	delegate.set(linkedTo);
	updateFromScene();
}


void TGen::Engine::Physics::Body::setKillTorque(bool killTorque) {
	this->killTorque = killTorque;
}


void TGen::Engine::Physics::Body::addForce(const TGen::Vector3 & force) {
	dBodyAddForce(bodyId, force.x, force.y, force.z);
}


void TGen::Engine::Physics::Body::addForceWorld(const TGen::Vector3 & absPos, const TGen::Vector3 & force) {
	dBodyAddForceAtPos(bodyId, force.x, force.y, force.z, absPos.x, absPos.y, absPos.z);
}


TGen::Vector3 TGen::Engine::Physics::Body::getLinearVelocity() const {
	//const dReal * force = dBodyGetLinearVel(bodyId);
	//return TGen::Vector3(force[0], force[1], force[2]);
	return linearVelocity;
}


TGen::Vector3 TGen::Engine::Physics::Body::getForce() const {
	const dReal * force = dBodyGetForce(bodyId);
	return TGen::Vector3(force[0], force[1], force[2]);	
}


void TGen::Engine::Physics::Body::setLinearDamping(scalar damping) {
	dBodySetLinearDamping(bodyId, damping);
}


void TGen::Engine::Physics::Body::setOnFloor(bool onFloor) {
	this->onFloor = onFloor;
}


bool TGen::Engine::Physics::Body::isOnFloor() const {
	return onFloor;
}


void TGen::Engine::Physics::Body::setSlope(scalar slope) {
	this->slope = slope;
}

// TODO: why doesn't scalar work as return type?
float TGen::Engine::Physics::Body::getSlope() const {
	return slope;
}


void TGen::Engine::Physics::Body::setFakeGravity(scalar fakeGrav) {
	this->fakeGrav = fakeGrav;
}


TGen::Vector3 TGen::Engine::Physics::Body::getVelocity() const {
	return getLinearVelocity();
}


void TGen::Engine::Physics::Body::setGroundNormal(const TGen::Vector3 & groundNormal) {
	this->groundNormal = groundNormal;
}


const TGen::Vector3 & TGen::Engine::Physics::Body::getGroundNormal() const {
	return groundNormal;
}


scalar TGen::Engine::Physics::Body::getGroundDefinition() const {
	return 0.4;
}


scalar TGen::Engine::Physics::Body::getMass() const {
	dMass mass;
	
	dBodyGetMass(bodyId, &mass);
	
	return mass.mass;
}


void TGen::Engine::Physics::Body::resetForces() {
	dBodySetForce(bodyId, 0.0f, 0.0f, 0.0f);
	dBodySetTorque(bodyId, 0.0f, 0.0f, 0.0f);
	dBodySetLinearVel(bodyId, 0.0f, 0.0f, 0.0f);
	dBodySetAngularVel(bodyId, 0.0f, 0.0f, 0.0f);
}


void TGen::Engine::Physics::Body::setEnabled(bool enabled) {
	if (enabled)
		dBodyEnable(bodyId);
	else
		dBodyDisable(bodyId);
}

