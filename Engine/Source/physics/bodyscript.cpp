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
	: TGen::Engine::Script::ComponentScript(name, entityScript, body)
	, body(body)
	, entityScript(entityScript)
{
	registerFunction("resetForces", luaResetForces);
	registerFunction("addForce", luaAddForce);
}

TGen::Engine::Physics::BodyScript::~BodyScript() {

}

void TGen::Engine::Physics::BodyScript::setEnabled(bool enabled) {
	body->setEnabled(enabled);
}

bool TGen::Engine::Physics::BodyScript::getEnabled() const {
	throw TGen::RuntimeException("bam", "bam");
	return false;
}

int TGen::Engine::Physics::BodyScript::luaResetForces(lua_State * vm) {
	TGen::Engine::Script::ScriptState scriptState(vm);
	
	BodyScript * self = scriptState.getSelfPointer<BodyScript *>();
	self->body->resetForces();
	
	return 0;
}

int TGen::Engine::Physics::BodyScript::luaAddForce(lua_State * vm) {
	TGen::Engine::Script::ScriptState scriptState(vm);
	
	BodyScript * self = scriptState.getSelfPointer<BodyScript *>();
	
	self->body->addForce(scriptState.toVector(2));
	
	return 0;	
}
