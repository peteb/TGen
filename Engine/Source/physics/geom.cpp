/*
 *  geom.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/12/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "physics/geom.h"
#include "physics/body.h"
#include "scene/node.h"

#include "entity.h"
#include "entitylist.h"

#include <tgen_core.h>
#include <tgen_renderer.h>

#include "triggerable.h"


using TGen::uint;

#define GEOM TGen::Engine::Physics::Geom

TGen::Engine::Symbol GEOM::symbolGetLink = TGen::Engine::getUniqueSymbol("getLink");

TGen::Engine::Physics::Geom::Geom(const std::string & name)
	: TGen::Engine::Component(name)
	, geomId(0)
	, affectsOthers(true)
	, categoryBits(0)
	, collidesWith(0)
	, collisionForceThreshold(3.0)
	, collisionForceScale(1.0)
{

}


TGen::Engine::Physics::Geom::~Geom() {
	dGeomDestroy(geomId);
}


float TGen::Engine::Physics::Geom::getFriction() const {
	return friction;
}


void TGen::Engine::Physics::Geom::setFriction(float friction) {
	this->friction = friction;
}


void TGen::Engine::Physics::Geom::setLink(const std::string & linkName) {
	bodyDelegate.set(linkName);
}


void TGen::Engine::Physics::Geom::setLink(TGen::Engine::WorldObject * linkedTo) {
	bodyDelegate.set(linkedTo);
}


void TGen::Engine::Physics::Geom::setCategory(uint category) {
	this->categoryBits = category;
	dGeomSetCategoryBits(geomId, categoryBits);
}


void TGen::Engine::Physics::Geom::setCollidesWith(uint collidesWith) {
	this->collidesWith = collidesWith;
	dGeomSetCollideBits(geomId, collidesWith);
}



void TGen::Engine::Physics::Geom::setGeomId(dGeomID id) {
	if (geomId)
		throw TGen::RuntimeException("Physics::Geom::setGeomId", "geomId already set!");
	
	geomId = id;
	dGeomSetData(geomId, static_cast<void *>(this));	
}


TGen::Engine::Physics::Body * TGen::Engine::Physics::Geom::getBody() {
	return bodyDelegate.getBodyObject();
}


void TGen::Engine::Physics::Geom::setBody(TGen::Engine::Physics::Body * body) {
	if (dGeomGetClass(geomId) != dPlaneClass) {
		dGeomSetBody(geomId, body->getBodyId());
	}
}


void TGen::Engine::Physics::Geom::link(const TGen::Engine::ComponentLinker & linker) {
	eventCollisionForce.link(linker);
	eventCollision.link(linker);
	
	if (geomId == 0)
		return;
	
	if (dGeomGetClass(geomId) != dPlaneClass) {		// planes are non-movable
		bodyDelegate.link(linker);
		
		if (bodyDelegate.getBodyObject())
			dGeomSetBody(geomId, bodyDelegate.getBodyObject()->getBodyId());
		
		updateFromLink();
	}	
	
	TGen::Engine::Component::link(linker);
}


void TGen::Engine::Physics::Geom::setAffectsOthers(bool affectOthers) {
	affectsOthers = affectOthers;
}


bool TGen::Engine::Physics::Geom::getAffectsOthers() const {
	return affectsOthers;
}


void TGen::Engine::Physics::Geom::preStep() {	// update geom with scene node vars if we're linked to one (probably because there is no body)
	updateFromLink();
}


void TGen::Engine::Physics::Geom::postStep() {
	// Do nothing, without a body we can't be moved physically
}


void TGen::Engine::Physics::Geom::setPosition(const TGen::Vector3 & position) {
	if (geomId != 0)
		dGeomSetPosition(geomId, position.x, position.y, position.z);
}


void TGen::Engine::Physics::Geom::setOrientation(const TGen::Matrix3x3 & orientation) {
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
	
	dGeomSetRotation(geomId, matrix);
}


void TGen::Engine::Physics::Geom::updateFromLink() {
	if (bodyDelegate.getWorldObject()) {
		setPosition(bodyDelegate.getPosition());
		setOrientation(bodyDelegate.getOrientation());
	}	
}


void TGen::Engine::Physics::Geom::sendToLink() {
	
}


void TGen::Engine::Physics::Geom::onCollisionForce(scalar force, bool groundCollision) {
	if (eventCollisionForce) {
		//force -= collisionForceThreshold;
		//force = std::max(force, 0.0f);
		
		if (force - collisionForceThreshold > 0.00001) {
			TGen::Engine::TriggerContext * context = eventCollisionForce->context;
			TGenAssert(context);
			
			std::cout << "FORCE: " << force * collisionForceScale - collisionForceThreshold << std::endl;
			
			scalar fixedForce = force * collisionForceScale - collisionForceThreshold;
			
			context->setRegister(2, fixedForce);
			context->setRegister<int>(3, groundCollision);
			
			eventCollisionForce->trigger(*context, TGen::Engine::TriggerPrecise);
		}
	}
}


bool TGen::Engine::Physics::Geom::onCollision(TGen::Engine::Physics::Geom * with, dGeomID id, const dContact & contactInfo) {
	
	return true;
}

void TGen::Engine::Physics::Geom::postCollision(TGen::Engine::Physics::Geom * with, dGeomID id, const dContact & contactInfo) {	
	if (eventCollision) {
		TGen::Engine::TriggerContext * context = eventCollision->context;
		TGenAssert(context);
		
		context->setParameter(0, with);
		context->numParameters = 1;
		
		eventCollision->trigger(*context, TGen::Engine::TriggerPrecise);
	}
	
}

uint TGen::Engine::Physics::Geom::getCategory() const {
	return categoryBits;
}


void TGen::Engine::Physics::Geom::trigger(TGen::Engine::TriggerContext & context, TriggerMode mode) {
	TGen::Engine::Symbol function = context.getFunctionSymbol();
	
	if (function == symbolGetLink) {
		std::cout << "GET LINK" << std::endl;
		
		if (bodyDelegate.getBodyObject())
			*context.getRegister<TGen::Engine::Physics::Body **>(context.getReturnRegister()) = bodyDelegate.getBodyObject();
		else
			*context.getRegister<TGen::Engine::WorldObject **>(context.getReturnRegister()) = bodyDelegate.getWorldObject();			
	}
	else {
		TGen::Engine::Component::trigger(context, mode);
	}
}


void TGen::Engine::Physics::Geom::setEventCollisionForce(const std::string & eventName) {
	eventCollisionForce.set(eventName);
}


void TGen::Engine::Physics::Geom::setEventCollision(const std::string & eventName) {
	eventCollision.set(eventName);
}

