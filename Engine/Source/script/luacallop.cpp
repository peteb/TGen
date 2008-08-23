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
	, returnToRegister(-1)
{
	luaContext = subsystem.getLuaContext();
}


void TGen::Engine::Script::LuaCallOperation::trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode) {
	std::cout << "LUA CALL " << functionName << " ************** " << std::endl;
	
	
	lua_getglobal(luaContext, functionName.c_str());
	
	for (int i = 0; i < parameters.size(); ++i) {
		float value = context.getRegister<float>(parameters[i]);
		lua_pushnumber(luaContext, value);
	
		std::cout << "PUSHED " << value << std::endl;
	}
	
	
	int errorCode = 0;
	
	if (returnToRegister != -1) {
		errorCode = lua_pcall(luaContext, parameters.size(), 1, 0);
		float ret = lua_tonumber(luaContext, -1);
		
		context.setRegister<float>(returnToRegister, ret);
	}
	else {
		errorCode = lua_pcall(luaContext, parameters.size(), 0, 0);
	}
}

void TGen::Engine::Script::LuaCallOperation::setFunctionName(const std::string & functionName) {
	this->functionName = functionName;
}

void TGen::Engine::Script::LuaCallOperation::addReturnToRegister(int registerId) {
	this->returnToRegister = registerId;
}

void TGen::Engine::Script::LuaCallOperation::addParameterRegister(int registerId) {
	parameters.push_back(registerId);
}
