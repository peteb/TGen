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
#include "filesystem.h"
#include "file.h"
#include "lua/lua.hpp"

#include "script/entityscript.h"

TGen::Engine::Script::Subsystem::Subsystem(TGen::Engine::StandardLogs & logs, TGen::Engine::Filesystem & filesystem, const std::string & mapname)
	: logs(logs)
	, filesystem(filesystem)
	, componentFactory(*this)
{	
}


TGen::Engine::Script::Subsystem::~Subsystem() {
	//logs.info["script-"] << "closing lua context..." << TGen::endl;
}


TGen::Engine::Component * TGen::Engine::Script::Subsystem::createComponent(const std::string & name, TGen::Engine::Entity & entity, const TGen::PropertyTree & properties) {
	return componentFactory.createComponent(name, entity, properties);
}


TGen::Engine::ComponentRecipe * TGen::Engine::Script::Subsystem::createComponentRecipe(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties) {
	return componentFactory.createComponentRecipe(name, entityName, properties);
}

TGen::Engine::Script::EntityScript * TGen::Engine::Script::Subsystem::createScriptEntity(const std::string & name) {
	return new TGen::Engine::Script::EntityScript(name, *this);
}

TGen::Engine::Script::ScriptState & TGen::Engine::Script::Subsystem::getScriptState() {
	return scriptState;
}

void TGen::Engine::Script::Subsystem::executeScripts(const std::string & path) {
	std::vector<std::string> files;
	filesystem.enumerateFiles(path, files, true);
	
	for (int i = 0; i < files.size(); ++i) {
		TGen::auto_ptr<TGen::Engine::File> file = filesystem.openRead(files[i]);
		scriptState.loadScriptFile(file.get(), files[i]);
	}
}



