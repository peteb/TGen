/*
 *  entityscript.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 2/17/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include "script/entityscript.h"
#include "script/subsystem.h"
#include "script/componentscript.h"
#include "lua/lua.hpp"
#include <iostream>

TGen::Engine::Script::EntityScript::EntityScript(const std::string & name, TGen::Engine::Script::Subsystem & creator) 
	: name(name)
	, creator(creator)
{
	ScriptState & scriptState = creator.getScriptState();
	
	scriptState.getGlobal("entities");
	scriptState.newTable();
	
	scriptState.setUserData("_objectSelf", this);
	scriptState.setFunction("getName", luaGetName);
	
	scriptState.setField(-2, name);
	scriptState.pop(1);
}

TGen::Engine::Script::EntityScript::~EntityScript() {
	// TODO: erase table from lua
}

TGen::Engine::Script::Subsystem & TGen::Engine::Script::EntityScript::getCreator() {
	return creator;
}

TGen::Engine::Script::ComponentScript * TGen::Engine::Script::EntityScript::createScriptComponent(const std::string & name, void * self) {
	TGen::auto_ptr<TGen::Engine::Script::ComponentScript> scriptComponent = new TGen::Engine::Script::ComponentScript(this, name, creator);
	
	ScriptState & scriptState = creator.getScriptState();
	
	scriptState.getGlobal("entities");
	scriptState.getField(-1, this->name);
	scriptState.newTable();
	
	scriptState.setUserData("_objectSelf", self);
	
	scriptState.setField(-2, name);
	scriptState.pop(1);
	
	return scriptComponent.release();
}

std::string TGen::Engine::Script::EntityScript::getName() const {
	return name;
}

int TGen::Engine::Script::EntityScript::luaGetName(lua_State * vm) {
	ScriptState scriptState(vm);
	scriptState.getTableValue("_objectSelf");
		
	TGen::Engine::Script::EntityScript * entity = reinterpret_cast<TGen::Engine::Script::EntityScript *>(scriptState.toUserData(-1));
	
	scriptState.pushString(entity->name);

	return 1;
}

