/*
 *  generateline.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/12/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "generateline.h"

TGen::Engine::GenerateLine::GenerateLine(const std::string & text) {
	parse(text);
}

const std::string & TGen::Engine::GenerateLine::getName() const {
	return name;
}

const TGen::Engine::GenerateLine::ParameterMap & TGen::Engine::GenerateLine::getParameters() const {
	return parameters;
}

void TGen::Engine::GenerateLine::parse(const std::string & text) {
	std::cout << "parsing " << text << std::endl;
	
	if (text.substr(0, 4) != "gen:")
		throw TGen::RuntimeException("GenerateLine::parse", "does not begin with 'gen:'");
		
	int firstSpace = text.find(" ");
	if (firstSpace == std::string::npos)
		firstSpace = text.size();
	
	name = text.substr(4, firstSpace - 4);
	
	std::string parameterString = text.substr(firstSpace + 1, text.size() - firstSpace);

	Tokenizer tokenizer;
	TGen::TokenStream tokens;
	TGen::TokenStream::TokenList::iterator iter, endIter;
	
	tokenizer.tokenizeString(parameterString, tokens, false);
	iter = tokens.getFirstToken();
	endIter = tokens.getEndToken();
	
	for (; iter != endIter; ++iter) {
		if (!lastValue.empty()) {
			parameters.insert(ParameterMap::value_type(lastValue, iter->second));
			lastValue = "";
		}
		else {
			lastValue = iter->second;
		}
	}
	
	if (!lastValue.empty())
		throw TGen::RuntimeException("TGen::Engine::GenerateLine::parse", "values and types don't add up!");
}

TGen::Engine::GenerateLine::Tokenizer::Tokenizer() {
	autoAddTilNextIgnore = true;
	
	ignores.push_back(" ");
	ignores.push_back("\t");
	ignores.push_back("\r");
	
	quotes.push_back(StringPair("\"", "\""));
	quotes.push_back(StringPair("'", "'"));
	comments.push_back(StringPair("/*", "*/"));
}

std::string TGen::Engine::GenerateLine::getParameter(const std::string & name) const {
	ParameterMap::const_iterator iter = parameters.find(name);
	if (iter == parameters.end())
		throw TGen::RuntimeException("GenerateLine::getParameter", "parameter '" + name + "' does not exist!");
	
	return iter->second;
}

std::string TGen::Engine::GenerateLine::getParameter(const std::string & name, const std::string & def) const {
	try {
		return getParameter(name);
	}
	catch (...) {
		return def;
	}
}
