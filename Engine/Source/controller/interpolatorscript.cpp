/*
 *  interpolatorscript.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/16/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include "interpolatorscript.h"
#include "script/scriptstate.h"
#include "controller/interpolator.h"

TGen::Engine::Controller::InterpolatorScript::InterpolatorScript(const std::string & name, Interpolator & interpolator, TGen::Engine::Script::EntityScript * entityScript)
	: TGen::Engine::Script::ComponentScript(name, entityScript, &interpolator)
	, interpolator(interpolator)
{
	registerFunction("setSpeed", luaSetSpeed);
}

void TGen::Engine::Controller::InterpolatorScript::onReachedEnd() {
	TGenAssert(scriptComponent && scriptEntity);
	
	TGen::Engine::Script::ScriptState & scriptState = getScriptState(); 
	int startStackTop = scriptState.getStackTop();
	pushComponent(scriptState);
	
	scriptState.getField(-1, "onReachedEnd");
	
	if (!scriptState.isNil(-1)) {
		pushComponent(scriptState);
		scriptState.call(1, 0);
	}
	
	scriptState.pop(abs(scriptState.getStackTop() - startStackTop));
}

void TGen::Engine::Controller::InterpolatorScript::setEnabled(bool enabled) {
	interpolator.setEnabled(enabled);
}

bool TGen::Engine::Controller::InterpolatorScript::getEnabled() const {
	return interpolator.getEnabled();
}

int TGen::Engine::Controller::InterpolatorScript::luaSetSpeed(lua_State * vm) {
	TGen::Engine::Script::ScriptState scriptState(vm);
	
	InterpolatorScript * self = scriptState.getSelfPointer<InterpolatorScript *>();
	self->interpolator.setSpeed(scriptState.toNumber(2));
	
	return 0;
}
