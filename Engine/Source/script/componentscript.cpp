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
#include "component.h"
#include "lua/lua.hpp"

TGen::Engine::Script::ComponentScript::ComponentScript(const std::string & name, TGen::Engine::Script::EntityScript * entityScript, TGen::Engine::Component * component)
	: name(name)
	, creator(entityScript->getCreator())
	, entityScript(entityScript)
	, component(component)
{
	std::cout << name << " CS: " << this << std::endl;
	
	ScriptState & scriptState = entityScript->getCreator().getScriptState();
	
	
	if (component->getParent()) {
		if (component->getParent()->getScriptInterface())
			component->getParent()->getScriptInterface()->pushComponent(scriptState);
		else
			throw TGen::RuntimeException("ComponentScript::ComponentScript", "component is a child, but parent does not have a script interface");
	}
	else {
		entityScript->pushEntity(scriptState);	
	}
	
	scriptState.newTable();
	scriptState.setUserData("_objectSelf", this);
	
	scriptState.setField(-2, name);
	
	
	registerFunction("owner", luaOwner);
	registerFunction("name", luaName);
	registerFunction("enable", luaEnable);
	registerFunction("disable", luaDisable);
	registerFunction("isEnabled", luaIsEnabled);
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
	if (component->getParent()) {
		if (component->getParent()->getScriptInterface())
			component->getParent()->getScriptInterface()->pushComponent(scriptState);
		else
			throw TGen::RuntimeException("ComponentScript::pushComponent", "component is a child, but parent does not have a script interface");
	}
	else {
		pushEntity(scriptState);	
	}

	
	scriptState.getField(-1, this->name);	
	scriptState.remove(-2);
	
	if (scriptState.isNil(1))
		throw TGen::RuntimeException("Script::ComponentScript::pushComponent", "could not find component");
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

int TGen::Engine::Script::ComponentScript::luaEnable(lua_State * vm) {
	TGen::Engine::Script::ScriptState scriptState(vm);
	ComponentScript * self = scriptState.getSelfPointer<ComponentScript *>();
	
	self->setEnabled(true);
	
	return 0;
}

int TGen::Engine::Script::ComponentScript::luaIsEnabled(lua_State * vm) {
	TGen::Engine::Script::ScriptState scriptState(vm);
	ComponentScript * self = scriptState.getSelfPointer<ComponentScript *>();
	
	return self->getEnabled();
	
	return 1;
}


int TGen::Engine::Script::ComponentScript::luaDisable(lua_State * vm) {
	TGen::Engine::Script::ScriptState scriptState(vm);
	ComponentScript * self = scriptState.getSelfPointer<ComponentScript *>();
	
	self->setEnabled(false);
	
	return 0;
}
