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
	: timer(timer)
	, scriptEntity(entityScript)
	, name(name)
{
	TGenAssert(entityScript);
	scriptComponent = entityScript->createScriptComponent(name, this);
	
	scriptComponent->registerFunction("interval", luaInterval);
	scriptComponent->registerFunction("setInterval", luaSetInterval);
	scriptComponent->registerFunction("enable", luaEnable);
	scriptComponent->registerFunction("disable", luaDisable);
	scriptComponent->registerFunction("name", luaName);
}


void TGen::Engine::Utilities::TimerScript::tick() {
	TGenAssert(scriptComponent && scriptEntity);
	
	
	TGen::Engine::Script::ScriptState & scriptState = scriptComponent->beginComponentScript();	// TODO: check stack push/pop in this function, throw on endComponentScript if they don't match

	scriptState.getField(-1, "onTick");
	
	if (!scriptState.isNil(-1)) {
		scriptState.getGlobal("entities");
		scriptState.getField(-1, scriptEntity->getName());
		scriptState.remove(-2);

		scriptState.getField(-1, this->name);
		scriptState.remove(-2);

		scriptState.call(1, 0);
	}
	else {
		scriptState.pop(1);
	}	
	
	scriptComponent->endComponentScript();
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

int TGen::Engine::Utilities::TimerScript::luaName(lua_State * vm) {
	TGen::Engine::Script::ScriptState scriptState(vm);
	
	TimerScript * self = scriptState.getSelfPointer<TimerScript *>();
	scriptState.pushString(self->name);
	
	return 1;	
}
