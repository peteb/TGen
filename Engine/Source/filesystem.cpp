/*
 *  filesystem.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/27/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include <iostream>
#include <tgen_core.h>
#include "filesystem.h"
#include "physfs.h"
#include "file.h"

TGen::Engine::Filesystem::Filesystem(const char * argv0) {
	std::cout << "[vfs]: initializing... " << std::endl;
	
	if (!PHYSFS_init(argv0))
		throw TGen::RuntimeException("Filesystem::Filesystem", "failed to initialize: ") << PHYSFS_getLastError();
	
	std::cout << "[vfs]: base: " << PHYSFS_getBaseDir() << std::endl;
	std::cout << "[vfs]: initialized" << std::endl;
}

TGen::Engine::Filesystem::~Filesystem() {
	std::cout << "[vfs]: shutting down..." << std::endl;
	PHYSFS_deinit();
}

TGen::Engine::File * TGen::Engine::Filesystem::openRead(const std::string & path) {
	PHYSFS_File * file = PHYSFS_openRead(path.c_str());
	if (!file)
		throw TGen::RuntimeException("Filesystem::openRead", "file '" + path + "' failed to open: ") << PHYSFS_getLastError();
	
	return new TGen::Engine::File(file);
}

TGen::Engine::File * TGen::Engine::Filesystem::openWrite(const std::string & path) {
	PHYSFS_File * file = PHYSFS_openWrite(path.c_str());
	if (!file)
		throw TGen::RuntimeException("Filesystem::openWrite", "file '" + path + "' failed to open: ") << PHYSFS_getLastError();

	return new TGen::Engine::File(file);	
}

TGen::Engine::File * TGen::Engine::Filesystem::openAppend(const std::string & path) {
	PHYSFS_File * file = PHYSFS_openAppend(path.c_str());
	if (!file)
		throw TGen::RuntimeException("Filesystem::openAppend", "file '" + path + "' failed to open: ") << PHYSFS_getLastError();

	return new TGen::Engine::File(file);	
}

void TGen::Engine::Filesystem::addSearchPath(const std::string & path, bool override) {		
	if (!PHYSFS_addToSearchPath(path.c_str(), override))
		throw TGen::RuntimeException("Filesystem::addSearchPath", "failed to add '" + path + "' to search path: ") << PHYSFS_getLastError();
}

bool TGen::Engine::Filesystem::exists(const std::string & path) {
	return PHYSFS_exists(path.c_str());
}
