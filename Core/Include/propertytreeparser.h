/*
 *  propertytreeparser.h
 *  TGen Core
 *
 *  Created by Peter Backman on 9/2/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_PROPERTYTREEPARSER_H
#define _TGEN_PROPERTYTREEPARSER_H

#include "tokenizer.h"
#include "propertytree.h"

namespace TGen {
	enum PropertyTokens {
		PropertyTokenBlockStart = 20,
		PropertyTokenBlockEnd = 21,
		PropertyTokenEOL = 22,
		PropertyTokenNodeEnd = 23,
	};
	
	class PropertyTreeParser {
	public:
		TGen::PropertyTree parse(const char * code);
		
	private:
		void parseBlock(TGen::PropertyTree & node);
		void addLine(const std::vector<std::string> & line, TGen::PropertyTree & node);
		void step();
		
		TGen::TokenStream tokens;
		TGen::TokenStream::TokenList::iterator currentToken, endIter;		
	};
	
	class PropertyTreeTokenizer : public TGen::Tokenizer {
	public:
		PropertyTreeTokenizer();
		
		int getSpecialToken(char * text, TGen::TokenStream & stream);		
	};
	
} // !TGen

#endif // !_TGEN_PROPERTYTREEPARSER_H

