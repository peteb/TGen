/*
 *  filesystem.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/27/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include <tgen_core.h>
#include "filesystem.h"
#include "physfs/physfs.h"
#include "file.h"
#include "log.h"

TGen::Engine::Filesystem::Filesystem(const char * argv0, TGen::Engine::StandardLogs & logs) 
	: logs(logs)
{
	logs.info["vfs+"] << "initializing... " << TGen::endl;
	logs.info["vfs+"] << "   argv0: " << argv0 << TGen::endl;
	

	
	if (!PHYSFS_init(argv0))
		throw TGen::RuntimeException("Filesystem::Filesystem", "failed to initialize: ") << PHYSFS_getLastError();
	
	std::string userDir = PHYSFS_getUserDir();
	std::string writeDir = userDir;
	
	// TODO: BUGLY CODE!!!!!!
	
#ifdef _PLATFORM_OSX
	writeDir += "Library/Application Support/";

	if (!PHYSFS_setWriteDir(writeDir.c_str()))
		throw TGen::RuntimeException("Filesystem::Filesystem", "failed to set write dir: ") << PHYSFS_getLastError();

	if (!PHYSFS_mkdir("TGen"))
		throw TGen::RuntimeException("Filesystem::Filesystem", "failed to mkdir TGen");
	
	writeDir += "TGen/";
	
	std::string restOfFSBase;
	
	if (argv0[0] == '/')
		restOfFSBase = std::string(argv0).substr(strlen(PHYSFS_getBaseDir()));
	else
		restOfFSBase = argv0;
	
	std::string bundleName = restOfFSBase.substr(0, restOfFSBase.find("/"));
	
	base = bundleName + "/Contents/Resources/";
	
#elif defined(_PLATFORM_WIN)
	std::cout << writeDir << std::endl;
	writeDir += "Application Data\\";

	if (!PHYSFS_setWriteDir(writeDir.c_str()))
		throw TGen::RuntimeException("Filesystem::Filesystem", "failed to set write dir: ") << PHYSFS_getLastError();

	if (!PHYSFS_mkdir("TGen"))
		throw TGen::RuntimeException("Filesystem::Filesystem", "failed to mkdir TGen");
	
	writeDir += "TGen/";
#else

	if (!PHYSFS_setWriteDir(writeDir.c_str()))
		throw TGen::RuntimeException("Filesystem::Filesystem", "failed to set write dir: ") << PHYSFS_getLastError();

	if (!PHYSFS_mkdir(".tgen"))
		throw TGen::RuntimeException("Filesystem::Filesystem", "failed to mkdir .tgen");

	writeDir += ".tgen/";
#endif
	
	if (!PHYSFS_setWriteDir(writeDir.c_str()))
		throw TGen::RuntimeException("Filesystem::Filesystem", "failed to set write dir: ") << PHYSFS_getLastError();
	
	if (!PHYSFS_addToSearchPath(writeDir.c_str(), 0))
		throw TGen::RuntimeException("Filesystem::Filesystem", "failed to add write dir as search dir: ") << PHYSFS_getLastError();
	
	logs.info["vfs+"] << "   base: " << PHYSFS_getBaseDir() << TGen::endl;
	logs.info["vfs+"] << "   home: " << PHYSFS_getUserDir() << TGen::endl;
	logs.info["vfs+"] << "   extrabase: " << base << TGen::endl;
	
	if (PHYSFS_getWriteDir())
		logs.info["vfs+"] << "   writedir: " << PHYSFS_getWriteDir() << TGen::endl;
	else
		logs.info["vfs+"] << "   writedir: none" << TGen::endl;
		
	
	PHYSFS_Version compiled;
	PHYSFS_Version linked;
	
	PHYSFS_VERSION(&compiled);
	PHYSFS_getLinkedVersion(&linked);
	
	logs.info["vfs+"] << "   physfs linked version: " << int(linked.major) << "." << int(linked.minor) << "." << int(linked.patch) << TGen::endl;
	logs.info["vfs+"] << "   physfs compiled version: " << int(compiled.major) << "." << int(compiled.minor) << "." << int(compiled.patch) << TGen::endl;
	
	logs.info["vfs+"] << "initialized" << TGen::endl;	
	
}

TGen::Engine::Filesystem::~Filesystem() {
	logs.info["vfs-"] << "shutting down..." << TGen::endl;
	PHYSFS_deinit();
}

TGen::Engine::File * TGen::Engine::Filesystem::openRead(const std::string & path) {
	logs.info["vfs"] << "opening file '" << path << "' for reading..." << TGen::endl;
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
	if (!PHYSFS_addToSearchPath((base + path).c_str(), override))
		throw TGen::RuntimeException("Filesystem::addSearchPath", "failed to add '" + base + path + "' to search path: ") << PHYSFS_getLastError();
}

bool TGen::Engine::Filesystem::exists(const std::string & path) {
	return PHYSFS_exists(path.c_str());
}



/*void TGen::Engine::Filesystem::outputPath(const std::string & path) {
	char **rc = PHYSFS_enumerateFiles(path.c_str());
	char **i;
	
	for (i = rc; *i != NULL; i++) {
		printf("[%s]: ", (path + *i).c_str());
		if (PHYSFS_isDirectory((path + *i).c_str()))
			 printf("dir\n");
		else
			 printf("file\n");
			
	}
	
	PHYSFS_freeList(rc);
}*/

void TGen::Engine::Filesystem::enumerateFiles(const std::string & path, std::vector<std::string> & output, bool recurse) {	
	char **rc = PHYSFS_enumerateFiles(path.c_str());
	char **i;
	
	for (i = rc; *i != NULL; i++) {
		if (PHYSFS_isDirectory((path + *i).c_str())) {
			if (recurse)
				enumerateFiles(path + *i + "/", output, recurse);
		}
		else {
			output.push_back(path + *i);
		}		
	}
	
	// den vet rätt på filer och kataloger i första nivån, men i djupare så är allt dirs?
	// /info = file, /maps/testmap/entities = dir!!!!!!
	// men /materials/*.material, /shaders/*.shader funkar, de ses som filer....
	// den säger "is a dir" på allt som man matar in...! även fast filerna man skriver inte finns!!!!
	// enumrera /maps/testmap/ och man får testmap [DIR]
	// enumrera /maps/blahblah/ och man får testmap [DIR]
	// läsa /maps/testmap/entities och man får IS A DIR!
	
	PHYSFS_freeList(rc);	
}

