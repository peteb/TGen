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
#include "entity.h"
#include <tgen_core.h>
#include "scene/node.h"
#include <tgen_renderer.h>
#include "entitylist.h"
#include "triggerable.h"

using TGen::uint;

TGen::Engine::Physics::Geom::Geom(const std::string & name)
	: TGen::Engine::Component(name)
	, geomId(0)
	, affectsOthers(true)
	, linkedTo(NULL)
	, bodyLinked(NULL)
	, categoryBits(0)
	, collidesWith(0)
	, linkCollisionForce(NULL)
	, collisionForceThreshold(3.0)
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
	this->linkName = linkName;
}


void TGen::Engine::Physics::Geom::setLink(TGen::Engine::WorldObject * linkedTo) {
	this->linkedTo = linkedTo;
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


void TGen::Engine::Physics::Geom::linkLocally(TGen::Engine::Entity & entity) {
	if (geomId == 0)
		return;

	if (dGeomGetClass(geomId) != dPlaneClass) {
		TGen::Engine::Component * component = NULL;
		
		try {
			component = &entity.getComponent(linkName);
		}
		catch (...) {	// fall back on "sceneNode"
			component = &entity.getComponent("sceneNode");
		}
		
		TGen::Engine::Physics::Body * body = dynamic_cast<TGen::Engine::Physics::Body *>(component);
	
		if (body) {	// we don't need a link if we've got a body
			dGeomSetBody(geomId, body->getBodyId());
			linkedTo = NULL;
			bodyLinked = body;
		}
		else {
			linkedTo = dynamic_cast<TGen::Engine::WorldObject *>(component);
			bodyLinked = NULL;
			updateFromLink();			
		}
	}
}


TGen::Engine::Physics::Body * TGen::Engine::Physics::Geom::getBody() {
	return bodyLinked;
}


void TGen::Engine::Physics::Geom::setBody(TGen::Engine::Physics::Body * body) {
	if (dGeomGetClass(geomId) != dPlaneClass) {
		dGeomSetBody(geomId, body->getBodyId());
	}
}


void TGen::Engine::Physics::Geom::linkGlobally(TGen::Engine::EntityList & entities, TGen::Engine::Entity & entity) {
	if (!collisionForceName.empty()) {
		linkCollisionForce = dynamic_cast<TGen::Engine::Triggerable *>(entities.getComponent(collisionForceName, entity, std::nothrow));
		
		if (!linkCollisionForce)
			throw TGen::RuntimeException("Geom::linkGlobally", "failed to get triggerable " + collisionForceName);
	}
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
	if (linkedTo) {
		setPosition(linkedTo->getPosition());
		setOrientation(linkedTo->getOrientation());
	}	
}


void TGen::Engine::Physics::Geom::sendToLink() {
	
}


void TGen::Engine::Physics::Geom::onCollisionForce(scalar force) {
	if (linkCollisionForce) {
		force -= collisionForceThreshold;
		force = std::max(force, 0.0f);
		
		if (force > 0.00001) {
			TGen::Engine::TriggerContext * context = linkCollisionForce->context;
			assert(context);
			context->setRegister(0, force);
			linkCollisionForce->trigger(*context);
		}
	}
}


uint TGen::Engine::Physics::Geom::getCategory() const {
	return categoryBits;
}

void TGen::Engine::Physics::Geom::setLinkCollisionForce(const std::string & name) {
	collisionForceName = name;
}

void TGen::Engine::Physics::Geom::trigger(TGen::Engine::TriggerContext & context) {

}
