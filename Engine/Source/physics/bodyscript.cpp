/*
 *  bodyscript.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/28/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include "physics/bodyscript.h"
#include "physics/body.h"
#include "script/scriptstate.h"
#include "script/entityscript.h"
#include "script/componentscript.h"

TGen::Engine::Physics::BodyScript::BodyScript(const std::string & name, TGen::Engine::Physics::Body * body, TGen::Engine::Script::EntityScript * entityScript) 
	: name(name)
	, body(body)
	, entityScript(entityScript)
{
	TGenAssert(entityScript);
	scriptComponent = entityScript->createScriptComponent(name, this);
	
	scriptComponent->registerFunction("owner", luaOwner);
	scriptComponent->registerFunction("enable", luaEnable);

}

TGen::Engine::Physics::BodyScript::~BodyScript() {
	delete scriptComponent;
}

int TGen::Engine::Physics::BodyScript::luaOwner(lua_State * vm) {
	TGen::Engine::Script::ScriptState scriptState(vm);
	
	BodyScript * self = scriptState.getSelfPointer<BodyScript *>();
	
	scriptState.getGlobal("entities");
	scriptState.getField(-1, self->entityScript->getName());
	scriptState.remove(-2);
	
	return 1;
}

int TGen::Engine::Physics::BodyScript::luaEnable(lua_State * vm) {
	TGen::Engine::Script::ScriptState scriptState(vm);
	
	BodyScript * self = scriptState.getSelfPointer<BodyScript *>();
	self->body->setEnabled(true);
	
	return 0;
}
