/*
 *  maploader.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/29/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "maploader.h"
#include "map.h"
#include "log.h"
#include "file.h"
#include "filesystem.h"
#include "maptokenizer.h"

TGen::Engine::MapLoader::MapLoader(TGen::Engine::StandardLogs & logs, TGen::Engine::Filesystem & filesystem)
	: logs(logs)
	, filesystem(filesystem)
{
}

TGen::Engine::Map * TGen::Engine::MapLoader::createMap(const std::string & name, const std::string & filename, const TGen::Vector3 & origin) {
	logs.info["map"] << "loading map " << filename << "..." << TGen::endl;
	
	TGen::Engine::File * file = filesystem.openRead(filename);
	
	TGen::Engine::MapTokenizer tokenizer;
	tokenizer.tokenizeString(file->readAll(), tokens, false);
	currentToken = tokens.getFirstToken();
	endIter = tokens.getEndToken();
	
	TGen::Engine::Map * newMap = new TGen::Engine::Map(name, origin);
	
	parseGlobalBlock(newMap);
	
	delete file;
	
	return newMap;
}

void TGen::Engine::MapLoader::parseGlobalBlock(TGen::Engine::Map * map) {
	if (currentToken == endIter)
		throw TGen::RuntimeException("MapLoader::parseGlobalBlock", "no tokens!");
	
	if (currentToken->first != TGen::Engine::ProcTokenIdent)
		throw TGen::RuntimeException("MapLoader::parseGlobalBlock", "invalid ident!");
	
	step();
	
	while (currentToken != endIter) {
		if (currentToken->first == TGen::Engine::ProcTokenModel) {
			/*stepAndCheck();
			if (currentToken->first != ProcTokenBlockStart)
				throw TGen::RuntimeException("ProcParser::parseGlobalBlock", "model: no block start!");
			
			stepAndCheck();
			procmap->addModel(parseModelBlock());*/
		}
		else if (currentToken->first == TGen::Engine::ProcTokenIAP) {
			/*stepAndCheck();
			if (currentToken->first != ProcTokenBlockStart)
				throw TGen::RuntimeException("ProcParser::parseGlobalBlock", "iap: no block start!");
			
			stepAndCheck();
			parseIAPBlock(procmap);*/
		}
		else if (currentToken->first == TGen::Engine::ProcTokenNodes) {
			/*stepAndCheck();
			if (currentToken->first != ProcTokenBlockStart)
				throw TGen::RuntimeException("ProcParser::parseGlobalBlock", "nodes: no block start!");
			
			stepAndCheck();
			parseNodesBlock(procmap);*/
		}
		else {
			
		}
		
		if (currentToken != endIter)
			step();
	}	
}

void TGen::Engine::MapLoader::step() {
	tokens.stepToken(currentToken, endIter);
}
