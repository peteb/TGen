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
#include "entity.h"

#include <iostream>

TGen::Engine::Script::EntityScript::EntityScript(TGen::Engine::Entity & entity, TGen::Engine::Script::Subsystem & creator) 
	: entity(entity)
	, creator(creator)
{
	ScriptState & scriptState = creator.getScriptState();
	
	scriptState.getGlobal("entities");
	scriptState.newTable();
	
	scriptState.setUserData("_objectSelf", this);
	scriptState.setFunction("name", luaGetName);
	//scriptState.setFunction("worldInterface", luaGetWorldInterface);
	
	scriptState.setField(-2, entity.getName());
	scriptState.pop(1);
}

TGen::Engine::Script::EntityScript::~EntityScript() {
	// TODO: erase table from lua
}

TGen::Engine::Script::Subsystem & TGen::Engine::Script::EntityScript::getCreator() {
	return creator;
}

void TGen::Engine::Script::EntityScript::onCreation() {
	// TODO: optimize using the register table, look up based on 'this' pointer
	
	ScriptState & scriptState = creator.getScriptState();
	
	pushEntity(scriptState);
	scriptState.getField(-1, "onCreation");
	
	if (!scriptState.isNil(-1)) {
		pushEntity(scriptState);		
		scriptState.call(1, 0);
		scriptState.pop(2);
	}
	else {
		scriptState.pop(3);
	}	
}

void TGen::Engine::Script::EntityScript::pushEntity(TGen::Engine::Script::ScriptState & scriptState) {
	scriptState.getGlobal("entities");
	scriptState.getField(-1, entity.getName());
	scriptState.remove(-2);
	
	if (scriptState.isNil(-1))
		throw TGen::RuntimeException("EntityScript::pushEntity", "Entity table doesn't exist for '" + entity.getName() + "'.");
}

std::string TGen::Engine::Script::EntityScript::getName() const {
	return entity.getName();
}

void TGen::Engine::Script::EntityScript::registerWorldInterface(TGen::Engine::WorldObject * worldInterface) {
	ScriptState & scriptState = creator.getScriptState();

	pushEntity(scriptState);
	
	if (worldInterface)
		scriptState.pushWorldObject(worldInterface);
	else
		scriptState.pushNil();
	
	scriptState.setField(-2, "worldInterface");
}

int TGen::Engine::Script::EntityScript::luaGetName(lua_State * vm) {
	ScriptState scriptState(vm);
	EntityScript * self = scriptState.getSelfPointer<EntityScript *>();
	
	scriptState.pushString(self->entity.getName());

	return 1;
}

/*int TGen::Engine::Script::EntityScript::luaGetWorldInterface(lua_State * vm) {
	ScriptState scriptState(vm);
	EntityScript * self = scriptState.getSelfPointer<EntityScript *>();

	TGen::Engine::WorldObject * worldInterface = self->entity.getWorldInterface();
	
	if (worldInterface)
		scriptState.pushWorldObject(worldInterface);
	else
		scriptState.pushNil();
	
	return 1;
}
*/
