/*
 *  id4cmloader.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/17/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "physics/id4cmloader.h"
#include "physics/id4cmtokenizer.h"
#include "filesystem.h"
#include "file.h"

TGen::Engine::Physics::Id4CMLoader::Id4CMLoader(TGen::Engine::Filesystem & filesystem)
	: filesystem(filesystem)
{
}

TGen::Engine::Physics::Id4CMGeom * TGen::Engine::Physics::Id4CMLoader::createGeom(const std::string & path) {
	std::auto_ptr<TGen::Engine::File> file(filesystem.openRead(path));
	
	TGen::Engine::Physics::Id4CMTokenizer tokenizer;
	tokenizer.tokenizeString(file->readAll(), tokens, true);
	currentToken = tokens.getFirstToken();
	endIter = tokens.getEndToken();
	
	//std::auto_ptr<TGen::Engine::Map> newMap(new TGen::Engine::Map(name, origin));
	
	//parseGlobalBlock(newMap.get());
	
	
	//return newMap.release();
	
	
	throw TGen::RuntimeException("Blah", "blah");
	
	return NULL;
}

