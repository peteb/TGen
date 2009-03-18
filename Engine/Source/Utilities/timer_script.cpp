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
	, name(name)
{
	TGenAssert(entityScript);
	scriptComponent = entityScript->createScriptComponent(name, this);
	
	//scriptComponent->registerFunction("worldPosition", luaWorldPosition);
	//scriptComponent->registerFunction("localPosition", luaLocalPosition);
	//scriptComponent->registerFunction("setMaterial", luaSetMaterial);
}


void TGen::Engine::Utilities::TimerScript::tick() {
	TGenAssert(scriptComponent);
	
	
	TGen::Engine::Script::ScriptState & scriptState = scriptComponent->beginComponentScript();	// TODO: check stack push/pop in this function, throw on endComponentScript if they don't match

	scriptState.getField(-1, "onTick");
	
	if (!scriptState.isNil(-1)) {
		scriptState.getGlobal("entities");
		scriptState.getField(-1, "box1");	// TODO: use EntityScript::name
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
