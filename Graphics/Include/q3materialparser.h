/*
 *  q3materialparser.h
 *  TGen Graphics
 *
 *  Created by Peter Backman on 7/17/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_Q3MATERIALPARSER_H
#define _TGEN_Q3MATERIALPARSER_H

#include <tgen_core.h>

namespace TGen {
	class Material;
	class Technique;
	class PassList;
	class Pass;
	class PassTextureUnit;
	class WaveGenerator;
	
	enum Q3MaterialTokens {
		Q3MaterialTokenBlockStart = 20,
		Q3MaterialTokenBlockEnd = 21,
		Q3MaterialTokenEndOfLine = 26,	
		
	};
	
	
	class Q3MaterialParser {
	public:	
		Q3MaterialParser();
		~Q3MaterialParser();
		
		void parse(const char * code, std::list<TGen::Material *> & materials);
		
	private:
		void parseGlobalBlock();
		void parseMaterialBlock(TGen::Material * material, TGen::PassList * passes);
		void parsePassBlock(TGen::Pass * pass, TGen::PassTextureUnit * unit, TGen::PassList * lod, TGen::Material * material);
		std::string getStringToken(const std::string & name, bool ignorelf = false, bool quote = true);
		std::string getNumericToken(const std::string & name, bool ignorelf = false, bool quote = true);
		TGen::WaveGenerator * parseWaveGenerator();
		
		void stepToken();
		void stepOverLF();
		
		std::list<TGen::Material *> materials;
		TGen::TokenStream::TokenList::iterator currentToken, endIter;
		TGen::TokenStream tokens;
	};
	
	
	class Q3MaterialTokenizer : public TGen::Tokenizer {
	public:
		Q3MaterialTokenizer();
		
		int getSpecialToken(char * text, TGen::TokenStream & stream);
	};
	
} // !TGen

#endif // !_TGEN_Q3MATERIALPARSER_H
