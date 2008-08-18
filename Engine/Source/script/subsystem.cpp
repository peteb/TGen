/*
 *  subsystem.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/12/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "script/subsystem.h"
#include "log.h"
#include "filesystem.h"
#include "file.h"
#include "lua/lua.hpp"


TGen::Engine::Script::Subsystem::Subsystem(TGen::Engine::StandardLogs & logs, TGen::Engine::Filesystem & filesystem, const std::string & mapname)
	: logs(logs)
	, filesystem(filesystem)
	, componentFactory(*this)
{
	initializeLua(mapname);	
}


TGen::Engine::Script::Subsystem::~Subsystem() {
	logs.info["script-"] << "closing lua context..." << TGen::endl;
	lua_close(vm);
}


TGen::Engine::Script::Event * TGen::Engine::Script::Subsystem::createComponent(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties) {
	return componentFactory.createComponent(name, entityName, properties);
}


TGen::Engine::ComponentRecipe * TGen::Engine::Script::Subsystem::createComponentRecipe(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties) {
	return componentFactory.createComponentRecipe(name, entityName, properties);
}


lua_State * TGen::Engine::Script::Subsystem::getLuaContext() const {
	return vm;
}


void TGen::Engine::Script::Subsystem::initializeLua(const std::string & mapname) {
	logs.info["script+"] << "creating lua context..." << TGen::endl;	
	vm = lua_open();
	TGenAssert(vm);
	
	luaL_openlibs(vm);
	
	loadScripts("/scripts/");
	loadScripts("/maps/" + mapname + "/scripts/");
}


void TGen::Engine::Script::Subsystem::loadScripts(const std::string & path) {
	std::vector<std::string> files;
	filesystem.enumerateFiles(path, files, true);
	
	for (int i = 0; i < files.size(); ++i)
		loadScriptFile(files[i]);
}


void TGen::Engine::Script::Subsystem::loadScriptFile(const std::string & filename) {
	logs.info["script"] << "loading script file \"" << filename << "\"..." << TGen::endl;
	
	std::auto_ptr<TGen::Engine::File> file(filesystem.openRead(filename));
	
	int ret = lua_load(vm, LuaChunkReader, reinterpret_cast<void *>(file.get()), filename.c_str());
	
	if (ret != 0)
		throw TGen::RuntimeException("Script::Subsystem::loadScriptFile", "failed to load file \"" + filename + "\":\n") << lua_tostring(vm, -1);
	
	lua_call(vm, 0, 0);	// this creates all functions
}


const char * TGen::Engine::Script::LuaChunkReader(lua_State * vm, void * data, size_t * size) {
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
