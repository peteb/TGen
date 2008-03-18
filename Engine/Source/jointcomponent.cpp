/*
 *  jointcomponent.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 2/29/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "joint.h"
#include "body.h"
#include "entity.h"
#include "entitylist.h"

TGen::Engine::Physics::Joint::Joint(const std::string & name, dJointID jointId, const std::string & attachTo) 
	: TGen::Engine::Component(name)
	, jointId(jointId)
	, attachTo(attachTo)
	, body1(NULL)
	, body2(NULL)
{
	
}


TGen::Engine::Physics::Joint::~Joint() {
	dJointDestroy(jointId);
}

void TGen::Engine::Physics::Joint::linkLocally(TGen::Engine::Entity & entity) {
	body1 = dynamic_cast<TGen::Engine::Physics::Body *>(entity.getComponent("physBody"));
}

void TGen::Engine::Physics::Joint::linkGlobally(TGen::Engine::EntityList & entities) {
	if (!attachTo.empty())
		body2 = dynamic_cast<TGen::Engine::Physics::Body *>(entities.getEntity(attachTo)->getComponent("physBody"));
	else
		body2 = NULL;
	
	dBodyID bodyId1 = 0, bodyId2 = 0;
	
	if (body1)
		bodyId1 = body1->getBodyId();
	if (body2)
		bodyId2 = body2->getBodyId();
	
	dJointAttach(jointId, bodyId1, bodyId2);
	setSimAnchor(anchor);
}
// TODO: måste skicka med vilken typ av joint det är talan om  NEJ! egna klasser, galet
void TGen::Engine::Physics::Joint::setAnchor(const TGen::Vector3 & anchor) {
	this->anchor = anchor;
	setSimAnchor(anchor);
}

void TGen::Engine::Physics::Joint::setSimAnchor(const TGen::Vector3 & anchor) {
	dJointSetBallAnchor(jointId, anchor.x, anchor.y, anchor.z);	
}

