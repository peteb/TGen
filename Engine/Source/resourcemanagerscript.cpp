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

	
	// Create "resources" namespace
	scriptState.newTable();
	scriptState.setGlobal("resources");
	
	scriptState.getGlobal("resources");
	scriptState.setUserData("_objectSelf", this);
	

	registerFunction("material", luaMaterial);

	scriptState.pop(2);
	
	
	// Create "_material" class; used as metatable
	scriptState.newTable();
	scriptState.setGlobal("_material");
	
	scriptState.getGlobal("_material");
	registerFunction("getName", luaMaterialName);
	
	scriptState.pop(2);
}

void TGen::Engine::ResourceManagerScript::registerFunction(const std::string & name, int (*func) (lua_State *L)) {
	TGen::Engine::Script::ScriptState & scriptState = scriptSubsystem.getScriptState();
	
	scriptState.pushFunction(func);
	scriptState.setField(-2, name);
	
	//scriptState.pop(2);
}

int TGen::Engine::ResourceManagerScript::luaMaterial(lua_State * vm) {
	TGen::Engine::Script::ScriptState scriptState(vm);
	
	
	scriptState.getField(-2, "_objectSelf");	
	TGen::Engine::ResourceManagerScript * self = reinterpret_cast<TGen::Engine::ResourceManagerScript *>(scriptState.toUserData(-1));

	
	TGen::Material * material = self->resources.getMaterial(scriptState.toString(-2));
	//scriptState.pushUserData(material);
	
	// Create material object
	scriptState.newTable();			// TODO: opt with createtable
	
	scriptState.getGlobal("_material");
	if (scriptState.isNil(-1))
		exit(34);
	
	scriptState.setMetatable(-2);
	
	scriptState.pushUserData(material);
	scriptState.setField(-2, "_objectSelf");
	
	scriptState.pushFunction(luaMaterialName);
	scriptState.setField(-2, "name");
	
	return 1;
}

int TGen::Engine::ResourceManagerScript::luaMaterialName(lua_State * vm) {
	TGen::Engine::Script::ScriptState scriptState(vm);

	scriptState.getField(1, "_objectSelf");	
	TGen::Material * material = reinterpret_cast<TGen::Material *>(scriptState.toUserData(-1));

	if (!material)
		scriptState.generateError("Material is NULL");

	scriptState.pushString(material->getName());
	
	return 1;
}
