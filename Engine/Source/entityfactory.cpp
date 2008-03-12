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
#include "log.h"
#include <tgen_core.h>

TGen::Engine::EntityFactory::EntityFactory(TGen::Engine::StandardLogs & logs)
	: logs(logs)
{
	
}

TGen::Engine::EntityFactory::~EntityFactory() {
	
}

TGen::Engine::Entity * TGen::Engine::EntityFactory::createEntity(const TGen::PropertyTree & properties) {
	TGen::Engine::Entity * entity = new TGen::Engine::Entity(properties.getName());
	
	for (int i = 0; i < properties.getNumNodes(); ++i) {
		if (properties.getNode(i).getName() == "inherit") {
			// inherit all components
			// TODO: FIX!!!!!!!! how does overriding and adding values to components from the inherited base work?
		}
		else {
			SubsystemMap::iterator iter = subsystems.find(properties.getNode(i).getName());

			if (iter == subsystems.end()) {
				logs.warning["entfa"] << "no registered subsystem for component type '" << properties.getNode(i).getName() << "'" << TGen::endl;
			}
			else {
				TGen::Engine::Component * newComponent = iter->second->createComponent(entity, properties.getNode(i));
				if (newComponent)
					entity->addComponent(newComponent, newComponent->getName());
			}		
		}
	}
	
	entity->linkLocally();
	
	return entity;
}

void TGen::Engine::EntityFactory::registerSubsystem(const std::string & componentName, TGen::Engine::Subsystem * subsystem) {
	if (subsystems.find(componentName) != subsystems.end())
		throw TGen::RuntimeException("EntityFactory::registerSubsystem", "component type " + componentName + " already registered!");
	
	subsystems.insert(SubsystemMap::value_type(componentName, subsystem));
}
