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
#include "entityrecipe.h"
#include "componentrecipe.h"
#include "component.h"
#include "subsystem.h"
#include "log.h"
#include "objectreference.h"
#include <tgen_core.h>
#include "worldobject.h"
#include "script/subsystem.h"		// TODO: UGH

TGen::Engine::EntityFactory::EntityFactory(TGen::Engine::StandardLogs & logs)
	: logs(logs)
{
}


TGen::Engine::EntityFactory::~EntityFactory() {

}


TGen::Engine::Entity * TGen::Engine::EntityFactory::createEntity(const TGen::PropertyTree & properties) {	
	TGen::Engine::Entity * entity = new TGen::Engine::Entity(properties.getName());

	// TODO: script entity should maybe be a component? :> remember to remove the delete in dtor for entity
	TGen::Engine::Script::EntityScript * scriptInterface = dynamic_cast<TGen::Engine::Script::Subsystem *>(subsystems["script"])->createScriptEntity(*entity);
	entity->setScriptInterface(scriptInterface);
	
	
	TGen::PropertyTree props = properties;
	
	if (properties.getNumAttributes() > 0) {
		if (properties.getAttribute(0) == "extends") {
			ClassMap::iterator iter = classDefinitions.find(properties.getAttribute(1));
			if (iter == classDefinitions.end())
				throw TGen::RuntimeException("EntityFactory::createEntity", "Entity class '" + properties.getAttribute(1) + "' invalid");
			
			props = extendTree(iter->second, properties);
		}
	}
	
	for (int i = 0; i < props.getNumNodes(); ++i) {
		std::cout << "ADDING COMPONENTS " << i << std::endl;
		TGen::Engine::Component * newComponent = createComponent(props.getNode(i), *entity);		// TODO: remove entityName, just use Entity *.

		entity->addComponent(newComponent, newComponent->getName());
		std::cout << "COMPONENTS ADDED" << std::endl;
	}
	
	TGen::Engine::WorldObject * worldInterface = NULL;
	std::string worldInterfaceName = properties.getProperty("worldInterface", "");

	if (!worldInterfaceName.empty()) {
		worldInterface = dynamic_cast<TGen::Engine::WorldObject *>(&entity->getComponent(worldInterfaceName));
		
		if (!worldInterface)
			throw TGen::RuntimeException("EntityFactory::createEntity", "World interface of entity '" + properties.getName() + "' points to non-world object");
	}
	
	scriptInterface->registerWorldInterface(worldInterface);
	entity->setWorldInterface(worldInterface);
	
	//entity->link(TGen::Engine::ComponentLinker(NULL, NULL));
	
	return entity;
}


TGen::Engine::Component * TGen::Engine::EntityFactory::createComponent(const TGen::PropertyTree & properties, TGen::Engine::Entity & entity) const {
	SubsystemMap::const_iterator iter = subsystems.find(properties.getName());

	TGen::Engine::Component * ret = NULL;
	
	std::string componentName = properties.getName();
	if (properties.getNumAttributes() > 0)
		componentName = properties.getAttribute(0);
	
	if (iter != subsystems.end()) {
		ret = iter->second->createComponent(componentName, entity, properties);		
	}
	else {
		throw TGen::RuntimeException("EntityFactory::createComponent", "No registered subsystem for component type '") << properties.getName() << "'";

	}	
	
	//if (ret && ret->isStatic())
	//	ret->setOwner(entity);		// owner = NULL if the component should be cloned
	if (!ret)
		throw TGen::RuntimeException("EntityFactory::createComponent", "Failed to create component " + entity.getName() + ":" + componentName);
	
	return ret;
}


TGen::Engine::ComponentRecipe * TGen::Engine::EntityFactory::createComponentRecipe(const std::string & entityName, const TGen::PropertyTree & properties) const {
	TGen::Engine::ComponentRecipe * ret = NULL;
	SubsystemMap::const_iterator iter = subsystems.find(properties.getName());
	
	std::string componentName = properties.getName();
	if (properties.getNumAttributes() > 0)
		componentName = properties.getAttribute(0);	
	
	if (iter == subsystems.end()) {
		logs.warning["entfa"] << "no registered subsystem for component type '" << properties.getName() << "'" << TGen::endl;
	}	
	else {		
		ret = iter->second->createComponentRecipe(componentName, entityName, properties);
	}	
	
	return ret;	
}


TGen::PropertyTree TGen::Engine::EntityFactory::extendTree(const TGen::PropertyTree & base, const TGen::PropertyTree & entity) const {
	// entity overrides base
	TGen::PropertyTree ret = base;
	
	for (int i = 0; i < entity.getNumNodes(); ++i) {
		try {
			const TGen::PropertyTree & entityBranch = entity.getNode(i);
			const TGen::PropertyTree::PropertyMap & entityProperties = entity.getNode(i).getProperties();
			
			TGen::PropertyTree::AttributeList attributes;
			if (entityBranch.getNumAttributes() > 0)
				attributes.push_back(entityBranch.getAttribute(0));			
			
			TGen::PropertyTree & baseBranch = ret.getNode(entityBranch.getName(), attributes);			


			/*if (baseName != entityName) {
				ret.addNode(entity.getNode(i));
			}
			else {	// the components have the same name
			*/	for (TGen::PropertyTree::PropertyMap::const_iterator iter = entityProperties.begin(); iter != entityProperties.end(); ++iter) {
					baseBranch.addProperty(TGen::PropertyTree::Property(*iter));
				}
			//}
		}
		catch (...) {
			// branch not found in base
			ret.addNode(entity.getNode(i));
		}
	}
	
	return ret;
}


void TGen::Engine::EntityFactory::addClassEntity(const TGen::PropertyTree & properties) {
	TGen::PropertyTree props;
	
	std::cout << "CLASS: " << properties.getName() << std::endl;
	
	if (properties.getNumAttributes() > 2 && properties.getAttribute(1) == "extends") {
		ClassMap::iterator iter = classDefinitions.find(properties.getAttribute(2));
		if (iter == classDefinitions.end())
			throw TGen::RuntimeException("EntityFactory::addClassEntity", "Entity class '" + properties.getAttribute(2) + "' invalid");
			
		props = extendTree(iter->second, properties);
	}
	else {
		props = properties;
	}

	for (int i = 0; i < props.getNumNodes(); ++i)
		std::cout << props.getNode(i).getName() << std::endl;
	
	classDefinitions.insert(ClassMap::value_type(properties.getName(), props));
}


TGen::Engine::EntityRecipe * TGen::Engine::EntityFactory::createPrototypeEntity(const TGen::PropertyTree & properties) {
	std::auto_ptr<TGen::Engine::EntityRecipe> entity(new TGen::Engine::EntityRecipe(properties.getName()));

	entity->setWorldInterface(properties.getProperty("worldInterface", ""));

	TGen::PropertyTree props;
		
	if (properties.getNumAttributes() > 2 && properties.getAttribute(1) == "extends") {
		ClassMap::iterator iter = classDefinitions.find(properties.getAttribute(2));
		if (iter == classDefinitions.end())
			throw TGen::RuntimeException("EntityFactory::addPrototypeEntity", "Entity prototype '" + properties.getAttribute(2) + "' invalid");
		
		props = extendTree(iter->second, properties);
	}
	else {
		props = properties;
	}
	
	for (int i = 0; i < props.getNumNodes(); ++i) {
		TGen::Engine::ComponentRecipe * newRecipe = createComponentRecipe(entity->getName(), props.getNode(i));

		if (newRecipe)
			entity->addComponentRecipe(newRecipe, newRecipe->getName());
	}	
	
	return entity.release();
}


void TGen::Engine::EntityFactory::registerSubsystem(const std::string & componentName, TGen::Engine::Subsystem * subsystem) {
	if (subsystems.find(componentName) != subsystems.end())
		throw TGen::RuntimeException("EntityFactory::registerSubsystem", "Component type " + componentName + " already registered!");
	
	subsystems.insert(SubsystemMap::value_type(componentName, subsystem));
}
