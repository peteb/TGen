/*
 *  filesystemscript.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/12/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include "filesystemscript.h"
#include "script/scriptstate.h"
#include "script/subsystem.h"
#include "filesystem.h"
#include "file.h"

TGen::Engine::FilesystemScript::FilesystemScript(TGen::Engine::Filesystem & filesystem, TGen::Engine::Script::Subsystem & scriptSubsystem) 
	: filesystem(filesystem)
{
	TGen::Engine::Script::ScriptState & scriptState = scriptSubsystem.getScriptState();
	
	
	// Create "filesystem" namespace
	scriptState.newTable();
	scriptState.setGlobal("filesystem");

	scriptState.getGlobal("filesystem");
	scriptState.setUserData("_objectSelf", this);
	
	scriptState.getGlobal("filesystem");
	scriptState.pushFunction(luaOpenRead);
	scriptState.setField(-2, "openRead");
	
	scriptState.pop(2);

	// Create "_material" class; used as metatable.
	scriptState.newTable();
	scriptState.setGlobal("_file");
	
	scriptState.getGlobal("_file");
	scriptState.pushFunction(luaFileReadAll);
	scriptState.setField(-2, "all");

	scriptState.pushFunction(luaFileClose);
	scriptState.setField(-2, "__gc");
	
	scriptState.getGlobal("_file");
	scriptState.setField(-2, "__index");
	
	scriptState.pop(2);
	
}

TGen::Engine::FilesystemScript::~FilesystemScript() {
	
}

int TGen::Engine::FilesystemScript::luaOpenRead(lua_State * vm) {
	TGen::Engine::Script::ScriptState scriptState(vm);
	
	try {
		FilesystemScript * self = scriptState.getSelfPointer<FilesystemScript *>();
		std::string filename = scriptState.toString(2);
	
		TGen::auto_ptr<TGen::Engine::File> file = self->filesystem.openRead(filename);
		
		void * udata = scriptState.newUserData(sizeof(File *));
		
		scriptState.getGlobal("_file");
		if (scriptState.isNil(-1))
			throw TGen::RuntimeException("FilesystemScript::luaOpenRead", "_file table not defined");
		
		scriptState.setMetatable(-2);
		*reinterpret_cast<TGen::Engine::File **>(udata) = file.release();
		
		return 1;
	}
	catch (const TGen::RuntimeException & error) {
		scriptState.generateError(error);
	}
	
	return 0;
}

int TGen::Engine::FilesystemScript::luaFileClose(lua_State * vm) {
	TGen::Engine::Script::ScriptState scriptState(vm);	
	TGen::Engine::File * file = *reinterpret_cast<TGen::Engine::File **>(scriptState.toUserData(1));
	
	delete file;
	
	return 0;
}

int TGen::Engine::FilesystemScript::luaFileReadAll(lua_State * vm) {
	TGen::Engine::Script::ScriptState scriptState(vm);	
	TGen::Engine::File * file = *reinterpret_cast<TGen::Engine::File **>(scriptState.toUserData(1));

	if (file) {
		scriptState.pushString(file->readAll());
		return 1;
	}
	else {
		scriptState.generateError("Failed to read from file");
	}
}

