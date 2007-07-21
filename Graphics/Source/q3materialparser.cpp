/*
 *  q3materialparser.cpp
 *  TGen Graphics
 *
 *  Created by Peter Backman on 7/17/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "q3materialparser.h"
#include "material.h"
#include "technique.h"
#include "pass.h"
#include "generator.h"

TGen::Q3MaterialParser::Q3MaterialParser() {}
TGen::Q3MaterialParser::~Q3MaterialParser() {}

void TGen::Q3MaterialParser::Parse(const char * code, std::list<TGen::Material *> & materials) {
	TGen::Q3MaterialTokenizer tokenizer;	
	tokenizer.TokenizeString(code, tokens, false);
	
	currentToken = tokens.getFirstToken();
	endIter = tokens.getEndToken();

	ParseGlobalBlock();
	
	std::list<TGen::Material *>::iterator iter = this->materials.begin();
	for (; iter != this->materials.end(); ++iter) {
		materials.push_back(*iter);
	}	
}

void TGen::Q3MaterialParser::ParseGlobalBlock() {
	while (currentToken != endIter) {
		if (currentToken->first == TGen::TokenValueString || currentToken->first == TGen::TokenQuote) {
			std::string materialName = currentToken->second;
			StepOverLF();
			
			if (currentToken->first != TGen::Q3MaterialTokenBlockStart)
				throw TGen::RuntimeException("Q3MaterialParser::ParseGlobalBlock", "expecting block start, not '" + currentToken->second + "'!");
		
			TGen::Material * newMaterial = new TGen::Material(materialName);
			TGen::TechniqueList * newTechList = new TGen::TechniqueList;
			TGen::Technique * newTech = new TGen::Technique;
			TGen::PassList * newLod = new TGen::PassList;
			
			newTech->setPassList(newLod, 9);
			newTechList->setTechnique(newTech, 0);
			newMaterial->setSpecialization("default", newTechList);

			materials.push_back(newMaterial);
			
			StepToken();

			std::cout << "entering material block '" << materialName << "'..." << std::endl;
			
			ParseMaterialBlock(newMaterial, newLod);
			
			std::cout << "left material block" << std::endl;
		}
		else if (currentToken->first != TGen::Q3MaterialTokenEndOfLine) {
			throw TGen::RuntimeException("Q3MaterialParser::ParseGlobalBlock", "expecting material name, not '" + currentToken->second + "'!");
		}
	
		StepToken();
	}
	
	
}

void TGen::Q3MaterialParser::ParseMaterialBlock(TGen::Material * material, TGen::PassList * passes) {
	while (currentToken != endIter && currentToken->first != TGen::Q3MaterialTokenBlockEnd) {
		if (currentToken->first == TGen::Q3MaterialTokenBlockStart) {
			StepToken();
			
			TGen::Pass * newPass = new TGen::Pass;
			newPass->setShader("fixed");
			newPass->setDepthFunc("lequal");
			passes->addPass(newPass);
			
			TGen::PassTextureUnit * newUnit = new TGen::PassTextureUnit(0, "");
			
			std::cout << "entering pass block" << std::endl;
			ParsePassBlock(newPass, newUnit);
			std::cout << "left pass block" << std::endl;
			
			newPass->AddTextureUnit(newUnit);
		}
		else if (currentToken->first == TGen::TokenValueString || currentToken->first == TGen::TokenQuote) {
			// it's a parameter...
			std::string paramName = currentToken->second;
			StepToken();
			
			std::vector<std::string> values;
			
			while (currentToken->first != TGen::Q3MaterialTokenEndOfLine && currentToken != endIter && currentToken->first != TGen::Q3MaterialTokenBlockEnd) {
				values.push_back(currentToken->second);
				StepToken();
			}
			
			material->setParameter(paramName, values);
			//		void setParameter(const std::string & name, const std::vector<std::string> & values);

		}
		else if (currentToken->first != TGen::Q3MaterialTokenEndOfLine) {
			throw TGen::RuntimeException("Q3MaterialParser::ParseMaterialBlock", "not expecting '" + currentToken->second + "'!");			
		}		
		StepToken();
	}
	
	if (currentToken->first == TGen::Q3MaterialTokenBlockEnd)
		StepToken();
}

void TGen::Q3MaterialParser::ParsePassBlock(TGen::Pass * pass, TGen::PassTextureUnit * unit) {
	while (currentToken != endIter && currentToken->first != TGen::Q3MaterialTokenBlockEnd) {
		if (TGen::toUpper(currentToken->second) == "BLENDFUNC") {
			StepToken();
			std::string source, dest;
			source = getStringToken("pass.blendFunc: expecting string value for source");
			StepToken();
			
			if (currentToken->first == TGen::TokenValueString || currentToken->first == TGen::TokenQuote)
				dest = currentToken->second;
			
			pass->setBlendFunc(source, dest);			
		}
		else if (TGen::toUpper(currentToken->second) == "TCGEN") {
			StepToken();
			std::string gen = getStringToken("pass.tcGen: expecting string value for tex coord gen");
			
			unit->setTexCoordGen(gen, gen);
		}
		else if (TGen::toUpper(currentToken->second) == "TCMOD") {
			StepToken();
			std::string modType = TGen::toUpper(getStringToken("pass.tcMod: expecting string value for tex coord mod type"));
			StepToken();
			
			if (modType == "SCALE") {
				std::string offU, offV;
				float U, V;
				
				offU = "0" + getNumericToken("texunit.tcMod.scroll: expecting numeric value for U");
				StepToken();
				offV = "0" + getNumericToken("texunit.tcMod.scroll: expecting numeric value for V");
				
				U = TGen::lexical_cast<float>(offU);
				V = TGen::lexical_cast<float>(offV);

				TGen::TextureCoordScale * scaler = new TGen::TextureCoordScale((TGen::ScalarGenerator *)NULL, NULL, true);
				scaler->u = U;
				scaler->v = V;
				
				unit->AddTexCoordTransformer(scaler);
			}
			else if (modType == "SCROLL") {
				std::string offU, offV;
				float U, V;
				
				offU = "0" + getNumericToken("texunit.tcMod.scroll: expecting numeric value for U");
				StepToken();
				offV = "0" + getNumericToken("texunit.tcMod.scroll: expecting numeric value for V");

				U = TGen::lexical_cast<float>(offU);
				V = TGen::lexical_cast<float>(offV);
				
				TGen::TextureCoordTranslate * translator = new TGen::TextureCoordTranslate((TGen::ScalarGenerator *)NULL, NULL, true);
				translator->u = U;
				translator->v = V;
				unit->AddTexCoordTransformer(translator);
			}
			else {
				throw TGen::RuntimeException("Q3MaterialParser::ParsePassBlock", "tcMod type not supported '" + modType + "'!");
			}
		}
		else if (TGen::toUpper(currentToken->second) == "RGBGEN") {
			StepToken();
			StepToken();
			
		}
		else if (currentToken->second == "map") {
			StepToken();
			std::string texName = getStringToken("pass.map: expecting string value for texture name");
			
			unit->setTextureName(texName);
		}
		else if (currentToken->first != TGen::Q3MaterialTokenEndOfLine) {
			throw TGen::RuntimeException("Q3MaterialParser::ParsePassBlock", "unexpected symbol: '" + currentToken->second + "'");
		}
		
		StepToken();
	}

	if (currentToken->first == TGen::Q3MaterialTokenBlockEnd)
		StepToken();
}

std::string TGen::Q3MaterialParser::getStringToken(const std::string & name, bool ignorelf, bool quote) {
	std::string ret;
	
	if (ignorelf) {
		while(currentToken->first == TGen::Q3MaterialTokenEndOfLine) {
			tokens.NextToken(currentToken, endIter);
		}
	}
	
	if (currentToken->first == TGen::TokenValueString || currentToken->first == TGen::TokenQuote)
		ret = currentToken->second;
	else
		throw TGen::RuntimeException("Q3MaterialParser::getStringToken", name + ", not '" + currentToken->second + "' (") << currentToken->first << ")";
	
	return ret;
}

std::string TGen::Q3MaterialParser::getNumericToken(const std::string & name, bool ignorelf, bool quote) {
	std::string ret;
	
	if (ignorelf) {
		while(currentToken->first == TGen::Q3MaterialTokenEndOfLine) {
			tokens.NextToken(currentToken, endIter);
		}
	}
	
	if (currentToken->first == TGen::TokenValueNumeric)
		ret = currentToken->second;
	else
		throw TGen::RuntimeException("MaterialParser::getNumericTokenk", name + ", not '" + currentToken->second + "' (") << currentToken->first << ")";
	
	return ret;
}

void TGen::Q3MaterialParser::StepOverLF() {
	do {
		StepToken();
	} while(currentToken->first == TGen::Q3MaterialTokenEndOfLine);	
}

void TGen::Q3MaterialParser::StepToken() {
	tokens.NextToken(currentToken, endIter);
}

TGen::Q3MaterialTokenizer::Q3MaterialTokenizer() {
	autoAddTilNextIgnore = true;
	
	ignores.push_back(" ");
	ignores.push_back("\t");
	ignores.push_back("\r");
	
	quotes.push_back(StringPair("\"", "\""));
	comments.push_back(StringPair("//", "\n"));
	comments.push_back(StringPair("/*", "*/"));
		
	tokens["{"] = TGen::Q3MaterialTokenBlockStart;
	tokens["}"] = TGen::Q3MaterialTokenBlockEnd;
	tokens["\n"] = TGen::Q3MaterialTokenEndOfLine;
}

int TGen::Q3MaterialTokenizer::getSpecialToken(char * text, TGen::TokenStream & stream) {
	return 0;
}