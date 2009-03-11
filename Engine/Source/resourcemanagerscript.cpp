/*
 *  resourcemanagerscript.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/11/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include "resourcemanagerscript.h"
#include "resourcemanager.h"
#include "script/scriptstate.h"
#include "script/subsystem.h"

namespace TGen {
	class Material;
}

TGen::Engine::ResourceManagerScript::ResourceManagerScript(TGen::Engine::ResourceManager & resources, TGen::Engine::Script::Subsystem & scriptSubsystem)
	: resources(resources)
	, scriptSubsystem(scriptSubsystem)
{
	TGen::Engine::Script::ScriptState & scriptState = scriptSubsystem.getScriptState();

	scriptState.newTable();
	scriptState.setGlobal("resources");
	
	scriptState.getGlobal("resources");
	scriptState.setUserData("_objectSelf", this);
	scriptState.pop(1);
	

	registerFunction("material", luaMaterial);
}

void TGen::Engine::ResourceManagerScript::registerFunction(const std::string & name, int (*func) (lua_State *L)) {
	TGen::Engine::Script::ScriptState & scriptState = scriptSubsystem.getScriptState();
	
	scriptState.getGlobal("resources");
	
	scriptState.pushFunction(func);
	scriptState.setField(-2, name);
	
	//scriptState.pop(2);
}

int TGen::Engine::ResourceManagerScript::luaMaterial(lua_State * vm) {
	TGen::Engine::Script::ScriptState scriptState(vm);
	
	
	scriptState.getField(-2, "_objectSelf");	
	TGen::Engine::ResourceManagerScript * self = reinterpret_cast<TGen::Engine::ResourceManagerScript *>(scriptState.toUserData(-1));

	
	TGen::Material * material = self->resources.getMaterial(scriptState.toString(-2));
	scriptState.pushUserData(material);
	
	return 1;
}
