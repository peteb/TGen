/*
 *  tokenizer.h
 *  TGen Core
 *
 *  Created by Peter Backman on 7/4/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_TOKENIZER_H
#define _TGEN_TOKENIZER_H

#include <string>
#include <list>
#include <utility>
#include <map>
#include "types.h"
#include "error.h"

namespace TGen {
	enum BuiltinToken {
		TokenQuote = 1,
		TokenValueString = 2,
		TokenValueNumeric = 3,
	};
	
	enum MathToken {
		MathPlusToken = 10,
		MathMinusToken,
		MathEqualsToken,
		MathParenthesisBeginToken,
		MathParenthesisEndToken,		
	};
	
	struct Token {
		std::string value;
		uint type;
	};
	
	class TokenStream {	// ska kunna ha en position i tokenstreamen så man kan läsa relativt
	public:
		virtual ~TokenStream() {currentToken = tokens.begin(); }
		
		typedef std::list<std::pair<uint, std::string> > TokenList;
		
		virtual void AddToken(uint token, const std::string & value) {tokens.push_back(std::pair<uint, std::string>(token, value)); }
		
		void NextToken(TokenList::iterator & iter, TokenList::iterator & end) {
			if (iter == end)
				throw TGen::RuntimeException("TokenStream::NextToken", "Unexpected end of stream");
			
			iter++;
		}
		
		TokenList::iterator getFirstToken() {
			return tokens.begin();	
		}
		
		TokenList::iterator getEndToken() {
			return tokens.end();	
		}
		
		TokenList::iterator currentToken;
		TokenList tokens;
	};
	
	
	class Tokenizer {
	public:
		Tokenizer();
		virtual ~Tokenizer();
		
		void TokenizeString(const std::string & _str, TokenStream & stream, bool _verbose = false);
		void TokenizeString(const char * str, TokenStream & stream, bool _verbose = false);
		//void TokenizeFile(const TGen::Path & path, TokenStream & stream);
		int getIgnore(char * text);
		int getToken(char * text, TokenStream & stream);
		int getToken(char * text);
		int getStartOfComment(char * text);
		int getEndOfComment(char * text);
		int getStartOfQuote(char * text);
		int getEndOfQuote(char * text, TGen::TokenStream & stream);
		int getAutoAdd(char * text, TGen::TokenStream & stream);
		bool isNumeric(const std::string & text);
		
		virtual int getSpecialToken(char * text, TGen::TokenStream & stream) {return 0; }
		virtual bool ProcessSpecialToken(const std::string & value, TGen::TokenStream & stream) {return false; }	// handle a special token, like a value
		
		typedef std::pair<std::string, std::string> StringPair;
		typedef std::list<std::string> StringList;
		typedef std::list<std::pair<std::string, std::string> > StringPairList;
		typedef std::map<std::string, uint> StringUintMap;
		
		StringUintMap tokens;
		StringList ignores;
		StringPairList quotes, comments;
		
		std::string quote;
		std::string startOfQuote, startOfComment;
		
		bool inQuote, inComment, autoAddTilNextIgnore, inEscape;
	};
	
	
} // !TGen

#endif // !_TGEN_TOKENIZER_H
