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
	dBodySetMass(newBodyId, &mass);
	
	TGen::Engine::Physics::Body * newBody = new TGen::Engine::Physics::Body(name, newBodyId, subsystem.getWorldId(), space);
	
	dBodySetData(newBodyId, reinterpret_cast<void *>(newBody));
	
	subsystem.addBody(newBody);
	
	return newBody;
}

void TGen::Engine::Physics::BodyRecipe::link(const TGen::Engine::ComponentLinker & linker, TGen::Engine::EntityRecipe & prototype) {
	linkComponentNum = prototype.getComponentIndex(linkName);
}

void TGen::Engine::Physics::BodyRecipe::fastLinkConstructed(TGen::Engine::Component & constructed, TGen::Engine::Entity & entity) {
	TGen::Engine::WorldObject & object = dynamic_cast<TGen::Engine::WorldObject &>(entity.getComponent(linkComponentNum));
	
	dynamic_cast<TGen::Engine::Physics::Body &>(constructed).setLink(&object);
	
}

void TGen::Engine::Physics::BodyRecipe::setLink(const std::string & name) {
	linkName = name;
}

