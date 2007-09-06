/*
 *  propertytreeparser.cpp
 *  TGen Core
 *
 *  Created by Peter Backman on 9/2/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "propertytreeparser.h"

TGen::PropertyTreeTokenizer::PropertyTreeTokenizer() {
	autoAddTilNextIgnore = true;
	
	ignores.push_back(" ");
	ignores.push_back("\t");
	ignores.push_back("\r");
	//ignores.push_back("\n");
	
	quotes.push_back(StringPair("\"", "\""));
	comments.push_back(StringPair("//", "\n"));
	comments.push_back(StringPair("/*", "*/"));
	
	tokens["{"] = TGen::PropertyTokenBlockStart;
	tokens["}"] = TGen::PropertyTokenBlockEnd;
	tokens["\n"] = TGen::PropertyTokenEOL;
}


int TGen::PropertyTreeTokenizer::getSpecialToken(char * text, TGen::TokenStream & stream) {
	return 0;
}
//#include <iostream>

TGen::PropertyTree TGen::PropertyTreeParser::parse(const char * code) {
	TGen::PropertyTreeTokenizer tokenizer;
	tokenizer.tokenizeString(code, tokens, false);
	currentToken = tokens.getFirstToken();
	endIter = tokens.getEndToken();	
	
	TGen::PropertyTree root;
	parseBlock(root);
	
	return root;
}

void TGen::PropertyTreeParser::step() {
	tokens.stepToken(currentToken, endIter);
}

void TGen::PropertyTreeParser::parseBlock(TGen::PropertyTree & node) {
	std::vector<std::string> line;  // stack
	
	while (currentToken != endIter) {
//		std::cout << "loop '" << currentToken->second << "'" << std::endl;
		
		if (currentToken->first == TGen::PropertyTokenBlockStart) {
			step();
			TGen::PropertyTree newNode(line.at(0));
//			std::cout << "parsing block " << line.at(0) << std::endl;
			line.clear();
			parseBlock(newNode);
			node.addNode(newNode);
		}
		else if (currentToken->first == TGen::PropertyTokenBlockEnd) {
			if (!line.empty()) {
				addLine(line, node);
				line.clear();				
			}
			
			step();
//			std::cout << "leaving block" << std::endl;
			break;
		}
		else if (currentToken->first == TGen::PropertyTokenEOL) {
			if (!line.empty()) {
//				std::cout << "finishing line " << line.size() << std::endl;
				addLine(line, node);
				line.clear();
			}
		}
		else {
			line.push_back(currentToken->second);
//			std::cout << "adding param" << std::endl;
		}
		
		if (currentToken != endIter)
			step();
	}
	
	if (!line.empty())
		addLine(line, node);
}

// TODO: element i en nod, som en array. och så man kan iterera!

void TGen::PropertyTreeParser::addLine(const std::vector<std::string> & line, TGen::PropertyTree & node) {
	std::string name = line.at(0);
	std::string value = line.at(1);
	
	node.addProperty(TGen::PropertyTree::Property(name, value));
}
