/*
 *  scriptstate.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 2/18/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include <tgen_core.h>
#include <tgen_math.h>
#include "scriptstate.h"
#include "script/scriptppc.h"
#include "script/exception.h"
#include "lua/lua.hpp"
#include "file.h"
#include "filesystem.h"

TGen::Engine::Script::ScriptState::ScriptState(TGen::Engine::Filesystem & filesystem) 
	: owner(true)
{
	vm = lua_open();
	TGenAssert(vm);
	
	luaL_openlibs(vm);
	
	lua_newtable(vm);	
	lua_setglobal(vm, "entities");
	
	lua_pushlightuserdata(vm, &filesystem);
	lua_setfield(vm, LUA_REGISTRYINDEX, "filesystem");
	
	lua_pushcfunction(vm, luaImport);
	lua_setglobal(vm, "import");
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

void TGen::Engine::Script::ScriptState::setGlobal(const std::string & name) {
	lua_setglobal(vm, name.c_str());
}

void TGen::Engine::Script::ScriptState::getGlobal(const std::string & name) {
	lua_getglobal(vm, name.c_str());
}

void TGen::Engine::Script::ScriptState::newTable() {
	lua_newtable(vm);
}

void * TGen::Engine::Script::ScriptState::newUserData(int size) {
	return lua_newuserdata(vm, size);
}

void TGen::Engine::Script::ScriptState::pushUserData(void * data) {
	lua_pushlightuserdata(vm, data);
}

void TGen::Engine::Script::ScriptState::pushNil() {
	lua_pushnil(vm);
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
	return lua_touserdata(vm, index);	
}

std::string TGen::Engine::Script::ScriptState::toString(int index, bool callToString) {
	if (callToString && lua_istable(vm, index)) {
		lua_getfield(vm, index, "__tostring");
		
		if (!lua_isnil(vm, -1)) {
			lua_pushvalue(vm, index + (index < 0 ? -2 : 0));
			call(1, 1);
			
			std::string ret(lua_tostring(vm, -1));
			pop(2);
			
			return ret;
		}
		else {
			throw TGen::Engine::ScriptException("No __tostring defined in table");
		}
	}
	else {
		return std::string(lua_tostring(vm, index));
	}
}

void TGen::Engine::Script::ScriptState::pushString(const std::string & val) {
	lua_pushlstring(vm, val.c_str(), val.size());
}

void TGen::Engine::Script::ScriptState::pushVector(const TGen::Vector3 & vec) {
	lua_createtable(vm, 3, 3);

	lua_getglobal(vm, "vec3");
	if (lua_isnil(vm, -1))
		exit(34);
	
	lua_setmetatable(vm, -2);
	
	
	lua_pushnumber(vm, vec.x);
	lua_setfield(vm, -2, "x");
	
	lua_pushnumber(vm, vec.y);
	lua_setfield(vm, -2, "y");
	
	lua_pushnumber(vm, vec.z);
	lua_setfield(vm, -2, "z");
	
	
}

void TGen::Engine::Script::ScriptState::setMetatable(int index) {
	lua_setmetatable(vm, index);
}

int TGen::Engine::Script::ScriptState::getStackTop() {
	return lua_gettop(vm);
}

bool TGen::Engine::Script::ScriptState::isNil(int index) {
	return lua_isnil(vm, index);
}

bool TGen::Engine::Script::ScriptState::isString(int index) {
	return lua_isstring(vm, index);
}

bool TGen::Engine::Script::ScriptState::isNumber(int index) {
	return lua_isnumber(vm, index);
}

void TGen::Engine::Script::ScriptState::generateError() {
	lua_error(vm);
}

void TGen::Engine::Script::ScriptState::generateError(const std::string & desc) {
	lua_pushstring(vm, desc.c_str());
	lua_error(vm);
}

void TGen::Engine::Script::ScriptState::generateError(const TGen::RuntimeException & error) {
	std::string formatted;
	if (!error.getWhere().empty())
		formatted = "In '" + error.getWhere() + "':\n";
	
	generateError(formatted + error.getDescription());
}

void TGen::Engine::Script::ScriptState::remove(int index) {
	lua_remove(vm, index);
}

void TGen::Engine::Script::ScriptState::pushNumber(float number) {
	lua_pushnumber(vm, number);
}

void TGen::Engine::Script::ScriptState::pushBoolean(bool boolean) {
	lua_pushboolean(vm, boolean);
}

float TGen::Engine::Script::ScriptState::toNumber(int index) {
	return lua_tonumber(vm, index);
}

TGen::Vector3 TGen::Engine::Script::ScriptState::toVector(int index) {
	TGen::Vector3 ret;
	
	int fixedIndex = (index < 0 ? getStackTop() + index + 1 : index);
	getField(fixedIndex, "x");
	getField(fixedIndex, "y");
	getField(fixedIndex, "z");
	
	ret.z = toNumber(-1);
	ret.y = toNumber(-2);
	ret.x = toNumber(-3);
	
	pop(3);
	
	return ret;
}

TGen::Matrix3x3 TGen::Engine::Script::ScriptState::toMatrix3(int index) {
	TGen::Vector3 x, y, z;
	
	getField(index, "x");
	getField(index, "y");
	getField(index, "z");

	x = toVector(-3);
	y = toVector(-2);
	z = toVector(-1);

	pop(3);
	
	return TGen::Matrix3x3(x, y, z);
}
/*
 
 void TGen::Engine::Script::ScriptState::pushVector(const TGen::Vector3 & vec) {
 lua_createtable(vm, 3, 3);
 
 lua_getglobal(vm, "vec3");
 if (lua_isnil(vm, -1))
 exit(34);
 
 lua_setmetatable(vm, -2);
 
 
 lua_pushnumber(vm, vec.x);
 lua_setfield(vm, -2, "x");
 
 lua_pushnumber(vm, vec.y);
 lua_setfield(vm, -2, "y");
 
 lua_pushnumber(vm, vec.z);
 lua_setfield(vm, -2, "z");
 
 
 }
 */

void TGen::Engine::Script::ScriptState::pushMatrix3(const TGen::Matrix3x3 & mat) {
	lua_createtable(vm, 0, 3);
	
	lua_getglobal(vm, "mat3");
	if (lua_isnil(vm, -1))
		exit(34);
	
	lua_setmetatable(vm, -2);
	
	pushVector(mat.getX());
	lua_setfield(vm, -2, "x");

	pushVector(mat.getY());
	lua_setfield(vm, -2, "y");

	pushVector(mat.getZ());
	lua_setfield(vm, -2, "z");
}

void TGen::Engine::Script::ScriptState::pushWorldObject(TGen::Engine::WorldObject * object) {
	lua_createtable(vm, 0, 1);
	
	lua_getglobal(vm, "worldobj");
	if (lua_isnil(vm, -1))
		throw TGen::RuntimeException("ScriptState::pushWorldObject", "'worldobj' type not defined");
	
	lua_setmetatable(vm, -2);
	
	
	pushUserData(object);
	setField(-2, "_objectSelf");
}

void TGen::Engine::Script::ScriptState::executeString(const std::string & line) {
	luaL_loadstring(vm, line.c_str());
	call(0, 0);
}

void TGen::Engine::Script::ScriptState::call(int nargs, int nresults) {
	int ret = lua_pcall(vm, nargs, nresults, 0);
	
	if (ret != 0) {
		std::string errorName = toString(-1);
		std::string callStack;
		
		lua_Debug info;
		
		int level = 0;
		
		while (lua_getstack(vm, level, &info) != 0) {
			if (lua_getinfo(vm, "Sln", &info) == 0) {
				break;
			}
			else {
				callStack += TGen::lexical_cast<std::string>(level) + " ";
				
				if (info.name)	
					callStack += std::string(info.name) + "[" + std::string(info.namewhat) + "]";
				else
					callStack += info.short_src;
					
				if (info.currentline != -1)
					callStack += "@" + TGen::lexical_cast<std::string>(info.currentline);
				
				callStack += "\n";
				
				level++;
			}
		}

		std::string description = errorName;
		
		if (!callStack.empty()) {
			callStack = callStack.substr(0, callStack.size() - 1);
			description += "\n\nCallstack:\n" + callStack;
		}
		
		throw TGen::Engine::ScriptException(description);
	}
}

void TGen::Engine::Script::ScriptState::loadScriptFile(TGen::Engine::File * file, const std::string & name) {
	std::string contents = file->readAll();
	TGen::Engine::Script::ScriptPreprocessor ppc;
	std::string fixedContents = ppc.process(contents);

	std::string global = "local msg_send_self_l = msg_send_self;";//+ "local msg_send_l = msg_send;\n";
	
	fixedContents = global + fixedContents;
	
	//std::cout << name << ": " << fixedContents << std::endl;
	
	/*int ret = lua_load(vm, LuaChunkReader, reinterpret_cast<void *>(file), name.c_str());
	
	if (ret != 0)
		throw TGen::RuntimeException("Script::ScriptState::loadScriptFile", "Failed to load file \"" + name + "\":\n") << lua_tostring(vm, -1);
	*/
	
	//luaL_loadstring(vm, fixedContents.c_str());
	
	LuaMemory data;
	data.pos = 0;
	data.size = fixedContents.size();
	data.data = fixedContents.c_str();
	
	int ret = lua_load(vm, LuaMemoryReader, reinterpret_cast<void *>(&data), name.c_str());
	 
	if (ret != 0)
		 throw TGen::RuntimeException("Script::ScriptState::loadScriptFile", "Failed to load file \"" + name + "\":\n") << lua_tostring(vm, -1);
	
	
	
	call(0, 0);		// create everything in the file
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

const char * TGen::Engine::Script::ScriptState::LuaMemoryReader(lua_State * vm, void * data, size_t * size) {
	TGenAssert(data);
	
	LuaMemory * memory = reinterpret_cast<LuaMemory *>(data);
	
	if (memory->pos < memory->size) {
		int bytesLeft = memory->size - memory->pos;
		int bytesRead = std::min(bytesLeft, 4096);
		
		size_t lastPos = memory->pos;
		memory->pos += bytesRead;
		
		*size = bytesRead;

		return &memory->data[lastPos];
	}
	else {
		return NULL;
	}
	
	return NULL;
}

int TGen::Engine::Script::ScriptState::luaImport(lua_State * vm) {
	ScriptState scriptState(vm);
	std::string name = scriptState.toString(1);

	try {
		TGen::Engine::Filesystem * filesystem = scriptState.getFilesystem();
		TGenAssert(filesystem);
	
		TGen::auto_ptr<TGen::Engine::File> file = filesystem->openRead("/scripts/" + name);
	
		scriptState.loadScriptFile(file.get(), name);
	}
	catch (const TGen::RuntimeException & error) {
		scriptState.generateError(error);
	}
	
	return 0;
}

TGen::Engine::Filesystem * TGen::Engine::Script::ScriptState::getFilesystem() const {
	lua_getfield(vm, LUA_REGISTRYINDEX, "filesystem");
	
	return reinterpret_cast<TGen::Engine::Filesystem *>(lua_touserdata(vm, -1));
}



