/*
 *  luacallop.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/18/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "luacallop.h"
#include "lua/lua.hpp"
#include "script/subsystem.h"

TGen::Engine::Script::LuaCallOperation::LuaCallOperation(TGen::Engine::Script::EventOperation * parent, TGen::Engine::Script::Subsystem & subsystem) 
	: TGen::Engine::Script::EventOperation("LUA", parent)
	, subsystem(subsystem)
{
	luaContext = subsystem.getLuaContext();
}


void TGen::Engine::Script::LuaCallOperation::trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode) {
	std::cout << "LUA CALL " << functionName << " ************** " << std::endl;
	lua_getglobal(luaContext, functionName.c_str());
	lua_pcall(luaContext, 0, 0, 0);
}

void TGen::Engine::Script::LuaCallOperation::setFunctionName(const std::string & functionName) {
	this->functionName = functionName;
}
