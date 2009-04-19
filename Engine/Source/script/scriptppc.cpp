/*
 *  scriptppc.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 4/10/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include "script/scriptppc.h"
#include <iostream>

std::string TGen::Engine::Script::ScriptPreprocessor::process(const std::string & input) {
	std::string::size_type startpos = input.find("[");
	std::string::size_type prevpos = 0;
	
	std::string ret;
	
	while (startpos != std::string::npos) {
		std::string::size_type endpos = getFinalBracket(input, startpos);
		std::string test = input.substr(startpos, endpos - startpos);
		std::string processed = parseBlock(test);
		
		if (processed == "INVALID CASE")
			processed = test;
		
		ret += input.substr(prevpos, startpos - prevpos);
		ret += processed;
		prevpos = endpos;
		startpos = input.find("[", prevpos);
	}
	
	ret += input.substr(prevpos);
	
	return ret;
}

std::string::size_type TGen::Engine::Script::ScriptPreprocessor::getFinalBracket(const std::string & input, std::string::size_type start) {
	std::string::size_type pos = start;
	
	int bracketCount = 0;
	
	do {
		if (input[pos] == '[') {
			++bracketCount;
		}
		else if (input[pos] == ']') {
			if (--bracketCount == 0) {
				return pos + 1;
			}
		}
		
		++pos;
	} while(pos < input.size());
	
	return std::string::npos;
}

std::string TGen::Engine::Script::ScriptPreprocessor::parseBlock(const std::string & block) {
	std::cout << "PARSE BLOCK '" << block << "'" << std::endl;
	
	std::string::size_type pos = 0;
	std::string subblock;
	std::vector<std::string> elements;
	int ignoreBlock = 0;
	
	do {
		char atPos = block[pos];
		
		if (atPos == '(' || atPos == '{')
			ignoreBlock++;
		else if (atPos == ')' || atPos == '}')
			ignoreBlock--;
		
		if (atPos == '[') {
			std::string::size_type nextPos = getFinalBracket(block, pos);
			std::string newBlock = block.substr(pos + 1, nextPos - pos - 2);
			
			if (goodBlock(newBlock)) {
				subblock += parseBlock(newBlock);
				pos = nextPos - 1;
			}
			else {
				subblock += atPos;
			}
		}
		else if (atPos == ':' && elements.size() == 1/* && block.at(pos + 1) == ' '*/) {
			if (!subblock.empty()) {
				elements.push_back(subblock);
				subblock = "";
			}
			
			elements.push_back(":");
		}
		else if (ignoreBlock != 0 || (atPos != ' ' && atPos != '\t')) {
			subblock += atPos;
		}
		else if (!subblock.empty()) {
			elements.push_back(subblock);
			subblock = "";
		}
				
		++pos;
	} while (pos < block.size());
	
	if (!subblock.empty())
		elements.push_back(subblock);
	
	std::string ret = convertBlock(elements);

	return ret;
}

std::string TGen::Engine::Script::ScriptPreprocessor::convertBlock(const std::vector<std::string> & tokens) {
	std::cout << "CONVERT: " << std::endl;
	
	for (int i = 0; i < tokens.size(); ++i) {
		std::cout << "   '" << tokens[i] << "'" << std::endl;
	}
	
	if (tokens.size() == 1) {
		return tokens[0];
	}
	else if (tokens.size() == 2) {	// one receiver, one message
		std::string receiver = tokens[0];
		std::string message = getValueConverted(tokens[1]);
		
		//receiver = getValueConverted(receiver);
		
		return "msg_send_self_l(" + receiver + ", " + message + ")";
	}
	else if (tokens.size() > 3 && tokens[2] == ":") {	// one receiver, one message, one or more parameters
		std::string receiver = tokens[0];
		std::string message = getValueConverted(tokens[1]);
		std::string parameters;

		//receiver = getValueConverted(receiver);

		
		for (int i = 3; i < tokens.size(); ++i) {
			parameters += tokens[i]; //getValueConverted(tokens[i]); //tokens[i].substr(1, tokens[i].size() - 2);
			
			if (i < tokens.size() - 1)
				parameters += ", ";
		}
		
		return "msg_send_self_l(" + receiver + ", " + message + ", " + parameters + ")";
	}
	
	
	return "INVALID CASE";
}

std::string TGen::Engine::Script::ScriptPreprocessor::getValueConverted(const std::string & value) {
	bool strvalue = true;
	
	for (int i = 0; i < value.size(); ++i) {
		char atpos = value[i];
		
		if (atpos == ':' ||
			 atpos == '.' ||
			 atpos == '(' ||
			 atpos == ')' ||
			 atpos == '\"' ||
			 atpos == '\'')
			strvalue = false;
	}
	
	if (strvalue)
		return "'" + value + "'";
	
	return value;
}

bool TGen::Engine::Script::ScriptPreprocessor::goodBlock(const std::string & block) {
	std::cout << "CHECK BLOCK '" << block << "'" << std::endl;
	
	std::string part;
	std::vector<std::string> elements;
	for (int i = 0; i < block.size(); ++i) {
		char atpos = block[i];
		
		if (atpos == ' ' || atpos == '\t' && !part.empty()) {
			elements.push_back(part);
			part = "";
		}
		else {
			part += atpos;
		}
	}

	if (!part.empty())
		elements.push_back(part);

	
	if (elements.size() < 2)
		return false;
	
	return true;
}
