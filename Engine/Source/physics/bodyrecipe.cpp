/*
 *  bodyrecipe.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/24/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "bodyrecipe.h"
#include "entityrecipe.h"
#include "entity.h"
#include "physics/subsystem.h"
#include "physics/body.h"

TGen::Engine::Physics::BodyRecipe::BodyRecipe(const std::string & name, dSpaceID space, dMass mass, TGen::Engine::Physics::Subsystem & subsystem)
	: TGen::Engine::ComponentRecipe(name)
	, space(space)
	, mass(mass)
	, subsystem(subsystem)
	, linkComponentNum(-1)
{
}

TGen::Engine::Component * TGen::Engine::Physics::BodyRecipe::createComponent(const TGen::Engine::EntityRecipe & entity, TGen::Engine::Entity & constructing) {	
	dBodyID newBodyId = dBodyCreate(subsystem.getWorldId());

	TGen::Engine::Physics::Body * newBody = new TGen::Engine::Physics::Body(name, newBodyId, subsystem.getWorldId(), space);
	
	dBodySetData(newBodyId, reinterpret_cast<void *>(newBody));
	dBodySetMass(newBodyId, &mass);

	newBody->setPosition(TGen::Vector3::Zero);
	newBody->setTurnHeadwise(false);
	newBody->setMaxAngularSpeed(maxAngularSpeed);
	newBody->setKillTorque(killTorque);
	newBody->setLinearDamping(linearDamping);
	newBody->setFakeGravity(fakeGravity);
	newBody->setApplyGravity(applyGravity);
	newBody->setAutoDisable(autoDisable);
	
	//dBodySetGravityMode(newBodyId, false);

	
	subsystem.addBody(newBody);
	
	return newBody;
}

void TGen::Engine::Physics::BodyRecipe::link(const TGen::Engine::ComponentLinker & linker) {
	TGen::Engine::WorldObject * object = dynamic_cast<TGen::Engine::WorldObject *>(linker.getComponent(linkComponentNum));
	
	dynamic_cast<TGen::Engine::Physics::Body *>(linker.getComponent())->setLink(object);
}

void TGen::Engine::Physics::BodyRecipe::prelink(const TGen::Engine::ComponentLinker & linker) {
	linkComponentNum = linker.getEntityRecipe()->getComponentIndex(linkName);
}

void TGen::Engine::Physics::BodyRecipe::setLink(const std::string & name) {
	this->linkName = name;
}

void TGen::Engine::Physics::BodyRecipe::setMaxAngularSpeed(scalar speed) {
	this->maxAngularSpeed = speed;
}

void TGen::Engine::Physics::BodyRecipe::setKillTorque(bool killTorque) {
	this->killTorque = killTorque;
}

void TGen::Engine::Physics::BodyRecipe::setLinearDamping(scalar damping) {
	this->linearDamping = damping;
}

void TGen::Engine::Physics::BodyRecipe::setFakeGravity(scalar fakeGrav) {
	this->fakeGravity = fakeGrav;
}

void TGen::Engine::Physics::BodyRecipe::setApplyGravity(bool applyGravity) {
	this->applyGravity = applyGravity;
}

void TGen::Engine::Physics::BodyRecipe::setAutoDisable(bool autoDisable) {
	this->autoDisable = autoDisable;
}

/*
	TODO:
	istället för det här så ska det fungera såhär:
 
	link(entity, component, entitylist) utförs ALLTID när ett objekt har skapats
	recipes har en extra link (prelink) som istället löser ut index för länken, så link senare går på indexet
	prelink behöver bara en componentrecipe typ...
	Man ska göra allt det tunga i prelink!!

 
 
	Link:
		Vanliga komponenter:
			* EntityList
			* Entity
 
		Recipes:
			* Component
			* Entity
 
	Prelink:
		Recipes:
			* EntityList
			* ComponentRecipe
			
	*** KOlla vad som inte funkar nu... har glömt bort, fixa det, sen fixa länkning enligt ovan
 
	Adaptern stoppas in som en recipe i EntityList
	Static får bara referera static!!!!!! om en static redan blivit länkad så länka inte igen
 
	link betyder alltid att objektet har skapats, och det ska vara samma funktion för både Recipe och Component
	prelink finns bara hos Recipe och kallas från EntityList när alla recipes har skapats. dvs, när alla entiteter lästs in
	prelink kan komma åt att länka till andra entiteter, och att luska ut index för olika komponenter i receptet
	
 Gör det här bara... om det inte går så går det inte
 */


