/*
 *  maptokenizer.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/29/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "maptokenizer.h"

TGen::Engine::MapTokenizer::MapTokenizer() {
	autoAddTilNextIgnore = true;
	
	ignores.push_back(" ");
	ignores.push_back("\t");
	ignores.push_back("\r");
	ignores.push_back("\n");
	
	quotes.push_back(StringPair("\"", "\""));
	comments.push_back(StringPair("//", "\n"));
	comments.push_back(StringPair("/*", "*/"));
	
	tokens["{"] = ProcTokenBlockStart;
	tokens["}"] = ProcTokenBlockEnd;
	tokens["("] = ProcTokenArrayStart;
	tokens[")"] = ProcTokenArrayEnd;
	tokens["mapProcFile003"] = ProcTokenIdent;
	tokens["model"] = ProcTokenModel;
	tokens["interAreaPortals"] = ProcTokenIAP;
	tokens["nodes"] = ProcTokenNodes;	
}

int TGen::Engine::MapTokenizer::getSpecialToken(char * text, TGen::TokenStream & stream) {

	return 0;
}

