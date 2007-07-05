/*
 *  materialparser.h
 *  TGen Graphics
 *
 *  Created by Peter Backman on 7/4/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_MATERIALPARSER_H
#define _TGEN_MATERIALPARSER_H

#include <list>
#include "tgen_core.h"

namespace TGen {
	class Material;
	class Technique;
	class TechniqueList;
	class PassList;
	class Pass;
	
	enum MaterialTokens {
		MaterialTokenBlockStart = 20,
		MaterialTokenBlockEnd = 21,
		MaterialTokenMaterial = 22,
		MaterialTokenPass = 23,
		MaterialTokenLod = 24,
		MaterialTokenParameters = 25,	
		MaterialTokenEndOfLine = 26,	
		
	};
	
	class MaterialParser {
	public:
		MaterialParser();
		~MaterialParser();
		
		void Parse(const char * code, std::list<TGen::Material *> & materials);
		void ParseGlobalBlock();
		void ParseParamsBlock();
		void ParseMaterialBlock();
		void ParseLodBlock();
		void ParsePassBlock();
		void ParseTexunitBlock();
		
	private:
		TGen::Material * getMaterial(const std::string & name);
		std::string getStringToken(const std::string & name, bool ignorelf = false, bool quote = true);
		std::string getNumericToken(const std::string & name, bool ignorelf = false, bool quote = true);
		void StepToken();
		
		typedef std::map<std::string, TGen::Material *> MaterialMap;
		MaterialMap materials;
		
		TGen::Material * currentMaterial;
		TGen::Technique * currentTechnique;
		TGen::PassList * currentLod;
		TGen::Pass * currentPass;
		TGen::TechniqueList * currentSpecialization;
		TGen::TokenStream::TokenList::iterator currentToken, endIter;
		TGen::TokenStream tokens;
		
	};
	
	class MaterialTokenizer : public TGen::Tokenizer {
	public:
		MaterialTokenizer();
		
		int getSpecialToken(char * text, TGen::TokenStream & stream);
	};
	
} // !TGen

#endif // !_TGEN_MATERIALPARSER_H