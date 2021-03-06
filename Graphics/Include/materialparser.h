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
#include "color.h"

//#define _MATPARSER_VERBOSE

namespace TGen {
	class Material;
	class Technique;
	class TechniqueList;
	class PassList;
	class Pass;
	class PassTextureUnit;
	class WaveGenerator;
	
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
		
		void parse(const char * code, std::list<TGen::Material *> & materials);
		void addShaderPermutation(const std::string & params, int id);
		
	private:
		void parseGlobalBlock();
		void parseParamsBlock(TGen::Material * material);
		void parseMaterialBlock(TGen::Material * material);
		void parseLodBlock(TGen::PassList * lod, TGen::Material * material);
		void parsePassBlock(TGen::Pass * pass, TGen::PassList * lod, TGen::Material * material);
		void parseTexunitBlock(TGen::PassTextureUnit * unit, TGen::Material * material);
		
		TGen::WaveGenerator * parseWaveGenerator();		
		TGen::Color parseColor();
	
		TGen::Material * getMaterial(const std::string & name);
		std::string getStringToken(const std::string & name, bool ignorelf = false, bool quote = true);
		std::string getNumericToken(const std::string & name, bool ignorelf = false, bool quote = true);
		void stepToken();
		
		typedef std::map<std::string, TGen::Material *> MaterialMap;
		typedef std::pair<std::string, int> ShaderPermutation;
		typedef std::vector<ShaderPermutation> ShaderPermutationList;
		
		MaterialMap materials;
		ShaderPermutationList shaderPermutations;
		
		TGen::Technique * currentTechnique;
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
