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
			newTechList->setTechnique(newTech, 5);
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
			passes->addPass(newPass);
			
			std::cout << "entering pass block" << std::endl;
			
			ParsePassBlock(newPass);
			
			std::cout << "left pass block" << std::endl;
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

void TGen::Q3MaterialParser::ParsePassBlock(TGen::Pass * pass) {
	while (currentToken != endIter && currentToken->first != TGen::Q3MaterialTokenBlockEnd) {
		
		StepToken();
	}

	if (currentToken->first == TGen::Q3MaterialTokenBlockEnd)
		StepToken();
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