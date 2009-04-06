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

TGen::Engine::Physics::BodyScript::BodyScript(const std::string & name, TGen::Engine::Physics::Body * body, TGen::Engine::Script::EntityScript * entityScript) 
	: TGen::Engine::Script::ComponentScript(name, entityScript)
	, body(body)
	, entityScript(entityScript)
{
	registerFunction("enable", luaEnable);
	registerFunction("resetForces", luaResetForces);
}

TGen::Engine::Physics::BodyScript::~BodyScript() {

}

int TGen::Engine::Physics::BodyScript::luaEnable(lua_State * vm) {
	TGen::Engine::Script::ScriptState scriptState(vm);
	
	BodyScript * self = scriptState.getSelfPointer<BodyScript *>();
	self->body->setEnabled(true);
	
	return 0;
}

int TGen::Engine::Physics::BodyScript::luaResetForces(lua_State * vm) {
	TGen::Engine::Script::ScriptState scriptState(vm);
	
	BodyScript * self = scriptState.getSelfPointer<BodyScript *>();
	self->body->resetForces();
	
	return 0;
}
