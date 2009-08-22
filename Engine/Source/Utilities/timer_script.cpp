/*
 *  timer_script.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/27/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "utilities/timer_script.h"
#include "utilities/timer.h"
#include "script/entityscript.h"
#include "script/scriptstate.h"
#include "script/componentscript.h"

TGen::Engine::Utilities::TimerScript::TimerScript(const std::string & name, Timer & timer, TGen::Engine::Script::EntityScript * entityScript)
	: TGen::Engine::Script::ComponentScript(name, entityScript, &timer)
	, timer(timer)
	, scriptEntity(entityScript)
{
	registerFunction("interval", luaInterval);
	registerFunction("setInterval", luaSetInterval);
	registerFunction("enable", luaEnable);
	registerFunction("disable", luaDisable);
}


void TGen::Engine::Utilities::TimerScript::tick() {
	TGenAssert(scriptComponent && scriptEntity);
	
	TGen::Engine::Script::ScriptState & scriptState = getScriptState(); 
	int startStackTop = scriptState.getStackTop();
	pushComponent(scriptState);
	
	scriptState.getField(-1, "onTick");
	
	if (!scriptState.isNil(-1)) {
		pushComponent(scriptState);
		scriptState.call(1, 0);
	}
	
	scriptState.pop(abs(scriptState.getStackTop() - startStackTop));
}

int TGen::Engine::Utilities::TimerScript::luaInterval(lua_State * vm) {
	TGen::Engine::Script::ScriptState scriptState(vm);

	TimerScript * self = scriptState.getSelfPointer<TimerScript *>();
	scriptState.pushNumber(self->timer.getInterval());
	
	return 1;
}

int TGen::Engine::Utilities::TimerScript::luaSetInterval(lua_State * vm) {
	TGen::Engine::Script::ScriptState scriptState(vm);
	
	TimerScript * self = scriptState.getSelfPointer<TimerScript *>();
	self->timer.setInterval(scriptState.toNumber(2));
	
	return 0;
}

int TGen::Engine::Utilities::TimerScript::luaEnable(lua_State * vm) {
	TGen::Engine::Script::ScriptState scriptState(vm);
	
	TimerScript * self = scriptState.getSelfPointer<TimerScript *>();
	self->timer.setEnabled(true);
	
	return 0;
}

int TGen::Engine::Utilities::TimerScript::luaDisable(lua_State * vm) {
	TGen::Engine::Script::ScriptState scriptState(vm);
	
	TimerScript * self = scriptState.getSelfPointer<TimerScript *>();
	self->timer.setEnabled(false);
	
	return 0;
}


