/*
 *  envscript.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/26/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include "script/envscript.h"
#include "script/subsystem.h"
#include "platform_cocoa.h"		// TODO: include platform.h

TGen::Engine::Script::EnvironmentScript::EnvironmentScript(TGen::Engine::Script::ScriptState & scriptState) {
	scriptState.newTable();
	scriptState.setGlobal("env");
	
	scriptState.getGlobal("env");
	scriptState.pushFunction(luaShowMessage);
	scriptState.setField(-2, "showMessage");

	scriptState.pushFunction(luaQuit);
	scriptState.setField(-2, "quit");

	scriptState.pop(1);
}

TGen::Engine::Script::EnvironmentScript::~EnvironmentScript() {
	
}


int TGen::Engine::Script::EnvironmentScript::luaShowMessage(lua_State * vm) {
	TGen::Engine::Script::ScriptState scriptState(vm);
	
	std::string title, message;
	
	try {
		if (scriptState.isString(2)) {
			title = scriptState.toString(1);
			message = scriptState.toString(2);
		}
		else {
			title = "Message";
			message = scriptState.toString(1);
		}
	
		TGen::Engine::Platform::DisplayMessage(title, message);
	}
	catch (const TGen::RuntimeException & e) {
		scriptState.generateError(e);
	}
	
	return 0;
}

int TGen::Engine::Script::EnvironmentScript::luaQuit(lua_State * vm) {
	TGen::Engine::Script::ScriptState scriptState(vm);
	
	if (scriptState.isNumber(1))
		exit(int(scriptState.toNumber(1)));
	else
		exit(0);
	
	return 0;
}
