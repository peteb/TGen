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

TGen::Engine::Script::ComponentScript::ComponentScript(TGen::Engine::Script::EntityScript * entityScript, const std::string & name, TGen::Engine::Script::Subsystem & creator)
	: name(name)
	, creator(creator)
	, entityScript(entityScript)
{
	
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

	scriptState.pop(2);
}

TGen::Engine::Script::ScriptState & TGen::Engine::Script::ComponentScript::beginComponentScript() {
	ScriptState & scriptState = creator.getScriptState();
	
	scriptState.getGlobal("entities");
	scriptState.getField(-1, entityScript->getName());
	scriptState.getField(-1, this->name);
	
	return scriptState;
}

void TGen::Engine::Script::ComponentScript::endComponentScript() {
	creator.getScriptState().pop(3);
}
