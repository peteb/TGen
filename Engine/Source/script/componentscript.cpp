/*
 *  componentscript.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 2/17/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include "script/componentscript.h"
#include "script/subsystem.h"
#include "script/entityscript.h"
#include "lua/lua.hpp"

TGen::Engine::Script::ComponentScript::ComponentScript(const std::string & name, TGen::Engine::Script::EntityScript * entityScript)
	: name(name)
	, creator(entityScript->getCreator())
	, entityScript(entityScript)
{
	std::cout << name << " CS: " << this << std::endl;
	
	ScriptState & scriptState = entityScript->getCreator().getScriptState();
	
	scriptState.getGlobal("entities");
	scriptState.getField(-1, entityScript->getName());
	
	scriptState.newTable();

	scriptState.setUserData("_objectSelf", this);
	
	scriptState.setField(-2, name);
	scriptState.pop(1);
	
	registerFunction("owner", luaOwner);
}

TGen::Engine::Script::ComponentScript::~ComponentScript() {
	// TODO: remove table here
	
	
}

void TGen::Engine::Script::ComponentScript::registerFunction(const std::string & name, int (*func) (lua_State *L)) {
	ScriptState & scriptState = creator.getScriptState();
	
	scriptState.getGlobal("entities");
	scriptState.getField(-1, entityScript->getName());
	scriptState.getField(-1, this->name);

	scriptState.pushFunction(func);
	scriptState.setField(-2, name);

	scriptState.pop(3);
}

TGen::Engine::Script::ScriptState & TGen::Engine::Script::ComponentScript::beginComponentScript() {
	ScriptState & scriptState = creator.getScriptState();
	
	scriptState.getGlobal("entities");
	scriptState.getField(-1, entityScript->getName());
	scriptState.getField(-1, this->name);
	
	beginTop = scriptState.getStackTop();
	
	return scriptState;
}

void TGen::Engine::Script::ComponentScript::endComponentScript() {
	if (beginTop != creator.getScriptState().getStackTop()) {
		throw TGen::RuntimeException("ComponentScript::endComponentScript", "stack push/pops don't match");
	}
	
	creator.getScriptState().pop(3);
}


int TGen::Engine::Script::ComponentScript::luaOwner(lua_State * vm) {
	TGen::Engine::Script::ScriptState scriptState(vm);
	
	ComponentScript * self = scriptState.getSelfPointer<ComponentScript *>();
	
	scriptState.getGlobal("entities");
	scriptState.getField(-1, self->entityScript->getName());
	scriptState.remove(-2);
	
	return 1;
}
