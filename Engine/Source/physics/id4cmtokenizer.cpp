/*
 *  id4cmtokenizer.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/17/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "physics/id4cmtokenizer.h"

TGen::Engine::Physics::Id4CMTokenizer::Id4CMTokenizer() {
	autoAddTilNextIgnore = true;
	
	ignores.push_back(" ");
	ignores.push_back("\t");
	ignores.push_back("\r");
	ignores.push_back("\n");
	
	quotes.push_back(std::make_pair("\"", "\""));
	comments.push_back(std::make_pair("//", "\n"));
	comments.push_back(std::make_pair("/*", "*/"));
	
	tokens["{"] = Id4CMTokenBlockStart;
	tokens["}"] = Id4CMTokenBlockEnd;
	tokens["("] = Id4CMTokenArrayStart;
	tokens[")"] = Id4CMTokenArrayEnd;
	/*tokens["mapProcFile003"] = ProcTokenIdent;
	tokens["model"] = ProcTokenModel;
	tokens["interAreaPortals"] = ProcTokenIAP;
	tokens["nodes"] = ProcTokenNodes;	
	*/
}

int TGen::Engine::Physics::Id4CMTokenizer::getSpecialToken(char * text, TGen::TokenStream & stream) {
	return 0;
}

