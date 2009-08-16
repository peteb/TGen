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
		else
			stepToken();
	}
	
	ret += "}\n";
	
	return ret;
}

std::string TGen::Engine::Q3ShaderConverter::parsePass() {
	std::string ret;
	
	ret = "\tpass RENDERER/writeAttributes:COLOR_MAP,TRANSFORM_TEX {\n";
	
	std::string mapPath;
	std::vector<std::string> mapParams;
	
	while (currentToken != endIter) {
		std::string param = TGen::toLower(currentToken->second);
		
		if (param == "map") {
			stepToken();
			mapPath = currentToken->second;
		}
		else if (param == "clampmap") {
			stepToken();
			mapPath = currentToken->second;
			mapParams.push_back("wrap clamp clamp");
		}
		else if (param == "blendfunc") {
			stepToken();
			std::string source = TGen::toLower(currentToken->second);
			stepToken();
			
			if (source == "blend") {
				ret += "\t\tblendFunc blend\n";
			}
			else {
				std::string dest = TGen::toLower(currentToken->second);
				
				if (source == "gl_one" && dest == "gl_one") {
					ret += "\t\tblendFunc additive\n";
				}
				else {
					source = ConvertBlendComponent(source);
					dest = ConvertBlendComponent(dest);
					
					if (!source.empty() && !dest.empty())
						ret += "\t\tblendFunc " + source + " " + dest + "\n";
					
				}
			}
		}
		else if (param == "tcmod") {
			stepToken();
			std::string type = currentToken->second;
			stepToken();
			
			if (TGen::toLower(type) != "stretch") {
				std::string first, second;
				
				if (currentToken->second != "wave") {
					first = currentToken->second;
					stepToken();
				}
				else {
					first = parseFunction();
				}

				if (currentToken->second != "wave") {
					second = currentToken->second;
					stepToken();
				}
				else {
					second = parseFunction();
				}
				
				mapParams.push_back(type + " " + first + " " + second);
			}
			else {
				mapParams.push_back(type + " " + parseFunction());
			}
		}
		else if (param == "rgbgen") {
			stepToken();
			std::string value = currentToken->second;
			stepToken();
			
			if (value == "wave")
				ret += "\t\tcolor " + parseFunction() + "\n";
			//else 
			//	ret += "\t\tcolor " + value + "\n";
		}
		else if (param == "alphagen") {
			stepToken();
			std::string value = currentToken->second;
			stepToken();
			
			if (value == "wave")
				ret += "\t\talpha " + parseFunction() + "\n";			
		}
		else if (currentToken->first == TGen::Q3MaterialTokenBlockEnd) {
			stepToken();
			break;
		}
		else
			stepToken();
	}
		
	if (!mapPath.empty()) {
		ret += "\t\tmap colorMap " + mapPath;
		
		if (!mapParams.empty()) {
			ret += " {\n";
			
			for (int i = 0; i < mapParams.size(); ++i) {
				ret += "\t\t\t" + mapParams[i] + "\n";
			}
			
			ret += "\t\t}\n";
		}
		else {
			ret += "\n";
		}
	}
	
	ret += "\t}\n";
	
	return ret;
}

std::string TGen::Engine::Q3ShaderConverter::ConvertBlendComponent(const std::string & comp) {
	//std::string fixed = TGen::toLower(comp);

	return comp;
}

std::string TGen::Engine::Q3ShaderConverter::parseFunction() {
	std::string function = currentToken->second;
	stepToken();
	std::string param1 = currentToken->second;
	stepToken();
	std::string param2 = currentToken->second;
	stepToken();
	std::string param3 = currentToken->second;
	stepToken();
	std::string param4 = currentToken->second;
	
	return "wave " + function + " " + param1 + " " + param2 + " " + param3 + " " + param4;
}

void TGen::Engine::Q3ShaderConverter::stepToken() {
	tokens.stepToken(currentToken, endIter);
}
