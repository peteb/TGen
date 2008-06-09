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

TGen::Engine::Physics::Geom::Geom(const std::string & name, const std::string & bodyComponent)
	: TGen::Engine::Component(name)
	, geomId(0)
	, bodyComponent(bodyComponent)
	, affectsOthers(true)
	, linkedTo(NULL)
{

}

TGen::Engine::Physics::Geom::~Geom() {
	
}

float TGen::Engine::Physics::Geom::getFriction() const {
	return friction;
}

void TGen::Engine::Physics::Geom::setFriction(float friction) {
	this->friction = friction;
}

void TGen::Engine::Physics::Geom::setLinkedComponent(const std::string & componentName) {
	bodyComponent = componentName;
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
			component = entity.getComponent(bodyComponent);
		}
		catch (...) {	// fall back on "sceneNode"
			component = entity.getComponent("sceneNode");
		}
		
		TGen::Engine::Physics::Body * body = dynamic_cast<TGen::Engine::Physics::Body *>(component);
	
		if (body) {	// we don't need a link if we've got a body
			dGeomSetBody(geomId, body->getBodyId());
			linkedTo = NULL;
		}
		else {
			linkedTo = dynamic_cast<TGen::Engine::WorldObject *>(component);
			updateFromLink();			
		}
	}
}

void TGen::Engine::Physics::Geom::linkGlobally(TGen::Engine::EntityList & entities) {
	
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

