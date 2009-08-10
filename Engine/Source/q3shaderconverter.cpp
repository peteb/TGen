/*
 *  q3shaderconverter.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/10/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include "q3shaderconverter.h"
#include <iostream>
#include <tgen_graphics.h>

std::string TGen::Engine::Q3ShaderConverter::convert(const std::string & data) {
	std::cout << "CONVERT" << std::endl << data << std::endl;

	TGen::Q3MaterialTokenizer tokenizer;
	tokenizer.ignores.push_back("\n");
	tokenizer.tokenizeString(data, tokens, true);
	
	currentToken = tokens.getFirstToken();
	endIter = tokens.getEndToken();

	std::string ret = parseGlobalBlock();
	
	std::cout << "\nresult:\n" << ret << std::endl;
	
	return ret;
}

std::string TGen::Engine::Q3ShaderConverter::parseGlobalBlock() {
	std::string ret;
	
	while (currentToken != endIter) {
		if (currentToken->first == TGen::TokenValueString) {
			std::string materialName = currentToken->second;
			stepToken();
			
			if (currentToken->first != TGen::Q3MaterialTokenBlockStart)
				throw TGen::RuntimeException("Q3ShaderConverter::parseGlobalBlock", "expected block start, not " + currentToken->second);
			
			stepToken();
			
			ret += parseMaterial(materialName);
		}
	}
	
	return ret;
}

std::string TGen::Engine::Q3ShaderConverter::parseMaterial(const std::string & name) {
	std::string ret;
	
	ret += "material " + name + " {\n";
	
	while (currentToken != endIter) {
		if (currentToken->first == TGen::Q3MaterialTokenBlockStart) {	// pass start
			ret += parsePass();
			
		}
		else if (currentToken->first == TGen::Q3MaterialTokenBlockEnd) {
			stepToken();
			break;
		}
		
		stepToken();
	}
	
	ret += "}\n";
	
	return ret;
}

std::string TGen::Engine::Q3ShaderConverter::parsePass() {
	std::string ret;
	
	ret = "\tpass RENDERER/writeAttributes:COLOR_MAP {\n";
	
	while (currentToken != endIter) {
		if (currentToken->second == "map") {
			stepToken();
			ret += "\t\tmap colorMap " + currentToken->second + "\n";
		}
		else if (currentToken->first == TGen::Q3MaterialTokenBlockEnd) {
			stepToken();
			break;
		}
		
		stepToken();
	}
	
	ret += "\t}\n";
	
	return ret;
}

void TGen::Engine::Q3ShaderConverter::stepToken() {
	tokens.stepToken(currentToken, endIter);
}
