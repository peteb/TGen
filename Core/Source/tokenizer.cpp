/*
 *  tokenizer.cpp
 *  TGen Core
 *
 *  Created by Peter Backman on 7/4/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "tokenizer.h"
#include <iostream>

TGen::Tokenizer::Tokenizer() : inQuote(false), inComment(false), inEscape(false) {}
TGen::Tokenizer::~Tokenizer() {}

// old code, but who cares.

void TGen::Tokenizer::TokenizeString(const std::string & _str, TGen::TokenStream & stream, bool _verbose) {
	TokenizeString(_str.c_str(), stream, _verbose);
}

void TGen::Tokenizer::TokenizeString(const char * str, TGen::TokenStream & stream, bool _verbose) {
	if (!str) return;
	
	char * currentChar = const_cast<char *>(str);
	
	while (*currentChar != '\0') {		
		if (!inQuote && !inComment) {
			int ignoreSize = getIgnore(currentChar);
			if (ignoreSize > 0) {
				currentChar += ignoreSize;
				
				if (_verbose)
					std::cout << "IGNORE ";
				
				continue;
			}
			
			int commentSize = getStartOfComment(currentChar);
			if (commentSize > 0) {
				currentChar += commentSize;
				inComment = true;
				
				if (_verbose)
					std::cout << "BEGINCOMMENT ";
				
				continue;
			}
			
			int quoteSize = getStartOfQuote(currentChar);
			if (quoteSize > 0) {
				if (!(currentChar != str && *(currentChar - 1) == '\\') || (currentChar != str)) {
					currentChar += quoteSize;
					inQuote = true;
					quote = "";
				
					if (_verbose)
						std::cout << "BEGINQUOTE ";
				}
				else
					currentChar += quoteSize;
				
				continue;				
			}
						
			int tokenSize = getToken(currentChar, stream);
			if (tokenSize > 0) {
				currentChar += tokenSize;
				
				if (_verbose)
					std::cout << "TOKEN ";
				
				continue;
			}
			
			int specialTokenSize = getSpecialToken(currentChar, stream);
			if (specialTokenSize > 0) {
				currentChar += specialTokenSize;
				
				if (_verbose)
					std::cout << "SPECIALTOKEN ";
				
				continue;
			}
			
			if (autoAddTilNextIgnore) {
				int autoAddSize = getAutoAdd(currentChar, stream);	
				if (autoAddSize > 0) {
					autoAddSize--;
					currentChar += autoAddSize;
					
					if (_verbose)
						std::cout << "AUTOADD: " << autoAddSize;
					
					continue;
				}
			}
		}
		else if (inQuote) {
			if (*currentChar == '\\' && *(currentChar + 1) != 'n') {
			//	inEscape = true;
				currentChar++;
				char buf[2] = {*currentChar, 0};
				quote += std::string(buf);	
				currentChar++;
				continue;
			}
						
			int endOfQuoteSize = getEndOfQuote(currentChar, stream);
			if (endOfQuoteSize > 0) {
				if (!(currentChar != str && *(currentChar - 1) == '\\')) {
					currentChar += endOfQuoteSize;
					stream.AddToken(TGen::TokenQuote, quote);
					inQuote = false;
					startOfQuote = "";
				
					if (_verbose)
						std::cout << "ENDQUOTE: '" << quote << "' ";
				}
				else
					currentChar += endOfQuoteSize;
				
				continue;
			}
			
			char buf[2] = {*currentChar, 0};
			quote += std::string(buf);	
		}
		else if (inComment) {
			int endOfCommentSize = getEndOfComment(currentChar);
			if (endOfCommentSize > 0) {
				currentChar -= getToken(currentChar);
				currentChar += endOfCommentSize;
				
				inComment = false;
				startOfComment = "";
				
				if (_verbose)
					std::cout << "ENDCOMMENT ";
				
				continue;
			}
		}
		
		currentChar++;
	}	
}

int TGen::Tokenizer::getIgnore(char * text) {
	if (!text) return 0;
	
	for (StringList::iterator it = ignores.begin(); it != ignores.end(); it++) {
		if (strncmp(text, (*it).c_str(), (*it).size()) == 0)
			return (*it).size();
	}
	
	return 0;
}

int TGen::Tokenizer::getToken(char * text, TGen::TokenStream & stream) {
	if (!text) return 0;
	
	for (StringUintMap::iterator it = tokens.begin(); it != tokens.end(); it++) {
		if (strncmp(text, it->first.c_str(), it->first.size()) == 0) {
			stream.AddToken(it->second, it->first);
			
			return it->first.size();
		}
	}

	return 0;
}

int TGen::Tokenizer::getToken(char * text) {
	if (!text) return 0;
	
	for (StringUintMap::iterator it = tokens.begin(); it != tokens.end(); it++) {
		if (strncmp(text, it->first.c_str(), it->first.size()) == 0)			
			return it->first.size();
	}
	
	return 0;
}
int TGen::Tokenizer::getStartOfComment(char * text) {
	if (!text) return 0;
	
	for (StringPairList::iterator it = comments.begin(); it != comments.end(); it++) {
		if (strncmp(text, it->first.c_str(), it->first.size()) == 0) {
			startOfComment = it->first;
			return it->first.size();
		}
	}
	
	return 0;
}

int TGen::Tokenizer::getEndOfComment(char * text) {
	if (!text) return 0;

	for (StringPairList::iterator it = comments.begin(); it != comments.end(); it++) {
		if (it->first == startOfComment) {
			if (strncmp(text, it->second.c_str(), it->second.size()) == 0) {
				return it->second.size();
			}
		}
	}
	
	return 0;	
}

int TGen::Tokenizer::getStartOfQuote(char * text) {
	if (!text) return 0;
	
	for (StringPairList::iterator it = quotes.begin(); it != quotes.end(); it++) {
		if (strncmp(text, it->first.c_str(), it->first.size()) == 0) {
			startOfQuote = it->first;
			return it->first.size();
		}
	}
	
	return 0;
}

int TGen::Tokenizer::getEndOfQuote(char * text, TGen::TokenStream & stream) {
	if (!text) return 0;
	
	for (StringPairList::iterator it = quotes.begin(); it != quotes.end(); it++) {
		if (it->first == startOfQuote) {
			if (strncmp(text, it->second.c_str(), it->second.size()) == 0)
				return it->second.size();
		}
	}
	
	return 0;
}

int TGen::Tokenizer::getAutoAdd(char * text, TGen::TokenStream & stream) {
	if (!text) return 0;
	
	std::string value;
	char * pos = text;
	int num = 0;
	while (*pos != '\0') {
		int ignoreSize = getIgnore(pos);
		int tokenSize = getToken(pos);
		
		if (tokenSize > 0)
			ignoreSize = tokenSize;
		
		if (ignoreSize > 0) {
			if (!ProcessSpecialToken(value, stream)) {
				if (!isNumeric(value))
					stream.AddToken(TokenValueString, value);
				else
					stream.AddToken(TokenValueNumeric, value);					
			}
			return num + ignoreSize;			
		}
		
		char buf[2] = {*pos, 0};
		value += std::string(buf);
		
		pos++;	
		num++;
	}
	
	return num;
}

bool TGen::Tokenizer::isNumeric(const std::string & text) {	// TODO: hexadec osv
	for (int i = 0; i < text.size(); i++) {
		if ((text[i] < '0' || text[i] > '9') && text[i] != '.' && text[i] != '-')			
			return false;
	}
	
	return true;	
}
