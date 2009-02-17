/*
 *  componentfactory.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/18/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include <tgen_core.h>
#include "script/componentfactory.h"

TGen::Engine::Script::ComponentFactory::ComponentFactory(TGen::Engine::Script::Subsystem & subsystem)
	: subsystem(subsystem)
{
}

TGen::Engine::ComponentRecipe * TGen::Engine::Script::ComponentFactory::createComponentRecipe(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties) {
	//TGen::Engine::Script::Event * newComponent = createComponent(name, entityName, properties);
	TGenAssert(0);
	
	return NULL; //new TGen::Engine::Script::EventRecipe(name, newComponent);	
}


TGen::Engine::Component * TGen::Engine::Script::ComponentFactory::createComponent(const std::string & name, TGen::Engine::Entity & entity, const TGen::PropertyTree & properties) {
	TGenAssert(0);
	return NULL;	
}

