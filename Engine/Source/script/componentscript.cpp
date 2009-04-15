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
	
	entityScript->pushEntity(scriptState);
	
	scriptState.newTable();
	scriptState.setUserData("_objectSelf", this);
	
	scriptState.setField(-2, name);
	
	
	registerFunction("owner", luaOwner);
	registerFunction("name", luaName);
}

TGen::Engine::Script::ComponentScript::~ComponentScript() {
	// TODO: remove table here
	
	
}

void TGen::Engine::Script::ComponentScript::registerFunction(const std::string & name, int (*func) (lua_State *L)) {
	ScriptState & scriptState = creator.getScriptState();
	
	pushComponent(scriptState);
	
	scriptState.pushFunction(func);
	scriptState.setField(-2, name);

	scriptState.pop(1);
}


void TGen::Engine::Script::ComponentScript::pushComponent(TGen::Engine::Script::ScriptState & scriptState) {
	pushEntity(scriptState);
	
	scriptState.getField(-1, this->name);	
	scriptState.remove(-2);
}

void TGen::Engine::Script::ComponentScript::pushEntity(TGen::Engine::Script::ScriptState & scriptState) {
	//entityScript->pushEntity(scriptState);
	scriptState.getGlobal("entities");
	scriptState.getField(-1, entityScript->getName());
	scriptState.remove(-2);
}

TGen::Engine::Script::ScriptState & TGen::Engine::Script::ComponentScript::getScriptState() const {
	return creator.getScriptState();
}

const std::string & TGen::Engine::Script::ComponentScript::getName() const {
	return name;
}

int TGen::Engine::Script::ComponentScript::luaOwner(lua_State * vm) {
	TGen::Engine::Script::ScriptState scriptState(vm);
	
	ComponentScript * self = scriptState.getSelfPointer<ComponentScript *>();
	
	scriptState.getGlobal("entities");
	scriptState.getField(-1, self->entityScript->getName());
	scriptState.remove(-2);
	
	return 1;
}

int TGen::Engine::Script::ComponentScript::luaName(lua_State * vm) {
	TGen::Engine::Script::ScriptState scriptState(vm);
	ComponentScript * self = scriptState.getSelfPointer<ComponentScript *>();
	
	scriptState.pushString(self->name);
	
	return 1;
}
