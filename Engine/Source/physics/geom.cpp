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
	, sceneNodeComponent(NULL)
	, attachedTo(NULL)
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

void TGen::Engine::Physics::Geom::setBodyComponent(const std::string & body) {
	bodyComponent = body;
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
	
	try {
		attachedTo = dynamic_cast<TGen::Engine::Physics::Body *>(entity.getComponent(bodyComponent));
		if (!attachedTo)
			throw TGen::RuntimeException("Geom::linkLocally", "not a body");
		
		dGeomSetBody(geomId, attachedTo->getBodyId());
	}
	catch (...) {	// if there is no physBody we just don't attach to anything
		dGeomSetBody(geomId, 0);
		
		try {
			if (dGeomGetClass(geomId) != dPlaneClass) {
				if (bodyComponent.empty() || bodyComponent == "physBody")
					bodyComponent = "sceneNode";
				
				sceneNodeComponent = dynamic_cast<TGen::Engine::Scene::Node *>(entity.getComponent(bodyComponent));
				
				setPosition(sceneNodeComponent->getSceneNode()->getLocalPosition());
			}
		}
		catch (...) {
			
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
	if (sceneNodeComponent) {
		setPosition(sceneNodeComponent->getSceneNode()->getWorldPosition());
		setOrientation(sceneNodeComponent->getSceneNode()->getLocalOrientation());
	}
}

void TGen::Engine::Physics::Geom::postStep() {
	// Do nothing, without a body we can't be manipulated
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


