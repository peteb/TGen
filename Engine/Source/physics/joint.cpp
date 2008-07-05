/*
 *  joint.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 2/29/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "physics/joint.h"
#include "physics/body.h"
#include "entity.h"
#include "entitylist.h"

TGen::Engine::Physics::Joint::Joint(const std::string & name, dJointID jointId) 
	: TGen::Engine::Component(name)
	, jointId(jointId)
	, body1(NULL)
	, body2(NULL)
{
	
}


TGen::Engine::Physics::Joint::~Joint() {
	dJointDestroy(jointId);
}


void TGen::Engine::Physics::Joint::linkGlobally(TGen::Engine::EntityList & entities, TGen::Engine::Entity & entity) {
	body1 = NULL;
	body2 = NULL;
	
	if (!link1Name.empty())
		body1 = &dynamic_cast<TGen::Engine::Physics::Body &>(entities.getComponent(link1Name, entity));
	
	if (!link2Name.empty())
		body2 = &dynamic_cast<TGen::Engine::Physics::Body &>(entities.getComponent(link2Name, entity));
	
	dBodyID bodyId1 = 0, bodyId2 = 0;
	
	if (body1)
		bodyId1 = body1->getBodyId();
	
	if (body2)
		bodyId2 = body2->getBodyId();
	
	dJointAttach(jointId, bodyId1, bodyId2);
	setSimAnchor(anchor);
}

// TODO: måste skicka med vilken typ av joint det är talan om  NEJ! egna klasser, galet. ska det vara olika klasser för olika jointtyper? kanske
void TGen::Engine::Physics::Joint::setAnchor(const TGen::Vector3 & anchor) {
	this->anchor = anchor;
	setSimAnchor(anchor);
}


void TGen::Engine::Physics::Joint::setSimAnchor(const TGen::Vector3 & anchor) {
	dJointSetBallAnchor(jointId, anchor.x, anchor.y, anchor.z);	
}


void TGen::Engine::Physics::Joint::setLink1(const std::string & link1Name) {
	this->link1Name = link1Name;
}


void TGen::Engine::Physics::Joint::setLink2(const std::string & link2Name) {
	this->link2Name = link2Name;
}

