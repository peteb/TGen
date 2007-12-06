/*
 *  entityfactory.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/5/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "entityfactory.h"
#include "entity.h"
#include "component.h"
#include "subsystem.h"
#include <tgen_core.h>

TGen::Engine::EntityFactory::EntityFactory() {
	
}

TGen::Engine::EntityFactory::~EntityFactory() {
	
}

TGen::Engine::Entity * TGen::Engine::EntityFactory::createEntity(const TGen::PropertyTree & properties) {
	TGen::Engine::Entity * entity = new TGen::Engine::Entity(properties["name"].second);
	
	for (int i = 0; i < subsystems.size(); ++i) {
		TGen::Engine::Component * newComponent = subsystems[i]->createComponent(entity, properties);
		if (newComponent)
			entity->addComponent(newComponent);
	}
	
	return entity;
}

void TGen::Engine::EntityFactory::registerSubsystem(TGen::Engine::Subsystem * subsystem) {
	subsystems.push_back(subsystem);
}
