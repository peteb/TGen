/*
 *  scriptstate.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 2/18/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include <tgen_core.h>
#include "scriptstate.h"
#include "lua/lua.hpp"
#include "file.h"

TGen::Engine::Script::ScriptState::ScriptState() 
	: owner(true)
{
	vm = lua_open();
	TGenAssert(vm);
	
	luaL_openlibs(vm);
	
	lua_newtable(vm);	
	lua_setglobal(vm, "entities");
}

TGen::Engine::Script::ScriptState::ScriptState(lua_State * prevm) 
	: owner(false)
{
	vm = prevm;
}

TGen::Engine::Script::ScriptState::~ScriptState() {
	if (owner)
		lua_close(vm);
}

lua_State * TGen::Engine::Script::ScriptState::getState() const {
	return vm;
}

void TGen::Engine::Script::ScriptState::getGlobal(const std::string & name) {
	lua_getglobal(vm, name.c_str());
}

void TGen::Engine::Script::ScriptState::newTable() {
	lua_newtable(vm);
}

void TGen::Engine::Script::ScriptState::pushUserData(void * data) {
	lua_pushlightuserdata(vm, data);
}

void TGen::Engine::Script::ScriptState::pushFunction(int (*func) (lua_State *L)) {
	lua_pushcfunction(vm, func);
}

void TGen::Engine::Script::ScriptState::setField(int index, const std::string & name) {
	lua_setfield(vm, index, name.c_str());
}

void TGen::Engine::Script::ScriptState::getField(int index, const std::string & name) {
	lua_getfield(vm, index, name.c_str());
}

void TGen::Engine::Script::ScriptState::pop(int count) {
	lua_pop(vm, count);
}

void TGen::Engine::Script::ScriptState::setUserData(const std::string & name, void * data) {
	pushUserData(data);
	setField(-2, name);
}

void TGen::Engine::Script::ScriptState::setFunction(const std::string & name, int (*func)(lua_State * L)) {
	pushFunction(func);
	setField(-2, name);
}

void TGen::Engine::Script::ScriptState::getTableValue(const std::string & name) {
	lua_pushstring(vm, name.c_str());
	lua_gettable(vm, -2);
}

void * TGen::Engine::Script::ScriptState::toUserData(int index) {
	return lua_touserdata(vm, -1);	
}

void TGen::Engine::Script::ScriptState::pushString(const std::string & val) {
	lua_pushstring(vm, val.c_str());
}

void TGen::Engine::Script::ScriptState::loadScriptFile(TGen::Engine::File * file, const std::string & name) {
	int ret = lua_load(vm, LuaChunkReader, reinterpret_cast<void *>(file), name.c_str());
	
	if (ret != 0)
		throw TGen::RuntimeException("Script::ScriptState::loadScriptFile", "failed to load file \"" + name + "\":\n") << lua_tostring(vm, -1);
	
	lua_call(vm, 0, 0);	// this creates all functions
}

const char * TGen::Engine::Script::ScriptState::LuaChunkReader(lua_State * vm, void * data, size_t * size) {
	TGenAssert(data);
	TGen::Engine::File * file = reinterpret_cast<TGen::Engine::File *>(data);
	
	static char * lastData = NULL;
	const int chunkSize = 4096;
	
	delete [] lastData;
	
	if (file->eof()) {
		lastData = NULL;
	}
	else {
		lastData = new char[chunkSize];
		
		int bytesRead = file->read(lastData, chunkSize);
		*size = bytesRead;
	}
	
	return lastData;
}
