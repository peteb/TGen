/*
 *  file.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/27/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "file.h"
#include "physfs.h"
#include <tgen_core.h>

TGen::Engine::File::File(PHYSFS_File * file)
	: file(file)
{
}

TGen::Engine::File::~File() {
	close();
}

void TGen::Engine::File::close() {
	if (file)
		PHYSFS_close(file);
	
	file = NULL;
}

std::string TGen::Engine::File::readAll() {
	PHYSFS_sint64 fileLength = PHYSFS_fileLength(file);
	if (fileLength == 0)
		return "";
	
	char * text = reinterpret_cast<char *>(malloc(fileLength + 1));
	if (!text)
		throw TGen::RuntimeException("File::readAll", "failed to allocate buffer");
	
	if (PHYSFS_read(file, text, fileLength, 1) < 0) {
		free(text);
		throw TGen::RuntimeException("File::readAll", "failed to read file: " + std::string(PHYSFS_getLastError()));
	}
		
	text[fileLength] = 0;
	std::string ret = text;
	
	free(text);	
	
	return ret;
}
