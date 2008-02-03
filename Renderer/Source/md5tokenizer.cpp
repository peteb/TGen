/*
 *  md5tokenizer.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 1/19/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "md5tokenizer.h"

TGen::MD5::Tokenizer::Tokenizer() {
	autoAddTilNextIgnore = true;
	
	ignores.push_back(" ");
	ignores.push_back("\t");
	ignores.push_back("\r");
	ignores.push_back("\n");
	
	quotes.push_back(StringPair("\"", "\""));
	comments.push_back(StringPair("//", "\n"));
	comments.push_back(StringPair("/*", "*/"));
		
	tokens["{"] = TokenBlockStart;
	tokens["}"] = TokenBlockEnd;
	tokens["("] = TokenArrayStart;
	tokens[")"] = TokenArrayEnd;
	tokens["MD5Version"] = TokenVersion;
	tokens["numJoints"] = TokenNumJoints;
	tokens["numMeshes"] = TokenNumMeshes;
	tokens["mesh"] = TokenMesh;
	tokens["joints"] = TokenJoints;
}
