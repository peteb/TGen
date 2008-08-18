/*
 *  subsystem.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/12/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "script/subsystem.h"
#include "log.h"

TGen::Engine::Script::Subsystem::Subsystem(TGen::Engine::StandardLogs & logs, const std::string & mapname)
	: logs(logs)
{
	std::string scriptFolder = "/maps/" + mapname + "/scripts/";
	
	logs.info["script+"] << "loading scripts from \"" << scriptFolder << "\"" << TGen::endl;
}

TGen::Engine::Script::Subsystem::~Subsystem() {
	
}


TGen::Engine::Script::Event * TGen::Engine::Script::Subsystem::createComponent(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties) {
	return componentFactory.createComponent(name, entityName, properties);
}

TGen::Engine::ComponentRecipe * TGen::Engine::Script::Subsystem::createComponentRecipe(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties) {
	return componentFactory.createComponentRecipe(name, entityName, properties);
}

