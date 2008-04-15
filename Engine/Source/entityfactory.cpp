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
	
	TGen::PropertyTree props = properties;
	
	if (properties.getNumAttributes() > 0) {
		if (properties.getAttribute(0) == "extends") {
			ClassMap::iterator iter = classDefinitions.find(properties.getAttribute(1));
			if (iter == classDefinitions.end())
				throw TGen::RuntimeException("EntityFactory::createEntity", "entity class '" + properties.getAttribute(1) + "' invalid");
			
			TGen::PropertyTree base = iter->second;
			props = extendTree(base, properties);
		}
	}
	
	for (int i = 0; i < props.getNumNodes(); ++i) {
		TGen::Engine::Component * newComponent = createComponent(entity->getName(), props.getNode(i));
		if (newComponent)
			entity->addComponent(newComponent, newComponent->getName());
		//else
		//	throw TGen::RuntimeException("EntityFactory::createEntity", "failed to create component");
	}
	
	entity->linkLocally();
	
	return entity;
}

TGen::Engine::Component * TGen::Engine::EntityFactory::createComponent(const std::string & entityName, const TGen::PropertyTree & properties) const {
	SubsystemMap::const_iterator iter = subsystems.find(properties.getName());
	
	if (iter == subsystems.end()) {
		logs.warning["entfa"] << "no registered subsystem for component type '" << properties.getName() << "'" << TGen::endl;
	}
	else {
		return iter->second->createComponent(entityName, properties);
	}	
	
	return NULL;
}

TGen::PropertyTree TGen::Engine::EntityFactory::extendTree(const TGen::PropertyTree & base, const TGen::PropertyTree & entity) const {
	TGen::PropertyTree ret = base;
	
	for (int i = 0; i < entity.getNumNodes(); ++i) {
		try {
			TGen::PropertyTree & baseBranch = ret.getNode(entity.getNode(i).getName());
			const TGen::PropertyTree::PropertyMap & entityProperties = entity.getNode(i).getProperties();
			
			for (TGen::PropertyTree::PropertyMap::const_iterator iter = entityProperties.begin(); iter != entityProperties.end(); ++iter) {
				baseBranch.addProperty(TGen::PropertyTree::Property(*iter));
				
			}
		}
		catch (...) {
			// branch not found in base
			ret.addNode(entity.getNode(i));
		}
	}
	
	return ret;
}

void TGen::Engine::EntityFactory::addClassEntity(const TGen::PropertyTree & properties) {
	classDefinitions.insert(ClassMap::value_type(properties.getName(), properties));
}

void TGen::Engine::EntityFactory::registerSubsystem(const std::string & componentName, TGen::Engine::Subsystem * subsystem) {
	if (subsystems.find(componentName) != subsystems.end())
		throw TGen::RuntimeException("EntityFactory::registerSubsystem", "component type " + componentName + " already registered!");
	
	subsystems.insert(SubsystemMap::value_type(componentName, subsystem));
}
