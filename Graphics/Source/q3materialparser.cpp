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
#include "techniquelist.h"
#include "pass.h"
#include "generator.h"
#include "passlist.h"

TGen::Q3MaterialParser::Q3MaterialParser() {}
TGen::Q3MaterialParser::~Q3MaterialParser() {}

void TGen::Q3MaterialParser::parse(const char * code, std::list<TGen::Material *> & materials) {
	TGen::Q3MaterialTokenizer tokenizer;	
	tokenizer.tokenizeString(code, tokens, false);
	
	currentToken = tokens.getFirstToken();
	endIter = tokens.getEndToken();

	parseGlobalBlock();
	
	std::list<TGen::Material *>::iterator iter = this->materials.begin();
	for (; iter != this->materials.end(); ++iter) {
		materials.push_back(*iter);
	}	
}

void TGen::Q3MaterialParser::parseGlobalBlock() {
	while (currentToken != endIter) {
		if (currentToken->first == TGen::TokenValueString || currentToken->first == TGen::TokenQuote) {
			std::string materialName = currentToken->second;
			stepOverLF();
			
			if (currentToken->first != TGen::Q3MaterialTokenBlockStart)
				throw TGen::RuntimeException("Q3MaterialParser::ParseGlobalBlock", "expecting block start, not '" + currentToken->second + "'!");
		
			TGen::Material * newMaterial = new TGen::Material(materialName);
			TGen::TechniqueList * newTechList = new TGen::TechniqueList("default");
			TGen::Technique * newTech = new TGen::Technique;
			TGen::PassList * newLod = new TGen::PassList;
			
			newTech->setPassList(newLod, 9);
			newTechList->setTechnique(newTech, 0);
			newMaterial->setSpecialization("default", newTechList);

			materials.push_back(newMaterial);
			
			stepToken();

			std::cout << "entering material block '" << materialName << "'..." << std::endl;
			
			parseMaterialBlock(newMaterial, newLod);
			
			std::cout << "left material block" << std::endl;
		}
		else if (currentToken->first != TGen::Q3MaterialTokenEndOfLine) {
			throw TGen::RuntimeException("Q3MaterialParser::ParseGlobalBlock", "expecting material name, not '" + currentToken->second + "'!");
		}
	
		stepToken();
	}	
}

void TGen::Q3MaterialParser::parseMaterialBlock(TGen::Material * material, TGen::PassList * passes) {
	while (currentToken != endIter && currentToken->first != TGen::Q3MaterialTokenBlockEnd) {
		if (currentToken->first == TGen::Q3MaterialTokenBlockStart) {
			stepToken();
			
			TGen::Pass * newPass = new TGen::Pass;
			newPass->setShader("fixed", 0);
			newPass->setDepthFunc("lequal");
			passes->addPass(newPass);
			
			TGen::PassTextureUnit * newUnit = new TGen::PassTextureUnit(0, "");
			
			std::cout << "entering pass block" << std::endl;
			parsePassBlock(newPass, newUnit, passes, material);
			std::cout << "left pass block" << std::endl;
			
			newPass->addTextureUnit(newUnit);
		}
		else if (currentToken->first == TGen::TokenValueString || currentToken->first == TGen::TokenQuote) {
			// it's a parameter...
			std::string paramName = currentToken->second;
			stepToken();
			
			std::vector<std::string> values;
			
			while (currentToken->first != TGen::Q3MaterialTokenEndOfLine && currentToken != endIter && currentToken->first != TGen::Q3MaterialTokenBlockEnd) {
				values.push_back(currentToken->second);
				stepToken();
			}
			
			material->setParameter(paramName, values);
			//		void setParameter(const std::string & name, const std::vector<std::string> & values);

		}
		else if (currentToken->first != TGen::Q3MaterialTokenEndOfLine) {
			throw TGen::RuntimeException("Q3MaterialParser::ParseMaterialBlock", "not expecting '" + currentToken->second + "'!");			
		}		
		
		stepToken();
	}
	
	if (currentToken->first == TGen::Q3MaterialTokenBlockEnd)
		stepToken();
}

void TGen::Q3MaterialParser::parsePassBlock(TGen::Pass * pass, TGen::PassTextureUnit * unit, TGen::PassList * lod, TGen::Material * material) {
	while (currentToken != endIter && currentToken->first != TGen::Q3MaterialTokenBlockEnd) {
		std::string tokenInLower = TGen::toLower(currentToken->second);
		
		if (tokenInLower == "blendfunc") {
			stepToken();
			std::string source, dest;
			source = getStringToken("pass.blendFunc: expecting string value for source");
			stepToken();
			
			if (currentToken->first == TGen::TokenValueString || currentToken->first == TGen::TokenQuote)
				dest = currentToken->second;
			
			lod->setSortLevel(TGen::MaterialSortTransparent); 
			pass->setBlendFunc(source, dest);			
		}
		else if (tokenInLower == "tcgen") {
			stepToken();
			std::string gen = getStringToken("pass.tcGen: expecting string value for tex coord gen");
			
			unit->setTexCoordGen(gen, gen);
		}
		else if (tokenInLower == "tcmod") {
			stepToken();
			std::string modType = TGen::toLower(getStringToken("pass.tcMod: expecting string value for tex coord mod type"));
			stepToken();
			
			if (modType == "scale" || modType == "stretch") {
				std::string u, v;
				float uNum = 0.0f, vNum = 0.0f;
				bool centered = modType == "stretch";
				
				TGen::ScalarGenerator * genU = NULL, * genV = NULL;
				
				if (currentToken->second == "sin" || currentToken->second == "saw" || currentToken->second == "inversesaw") {
					//std::cout << "SIN GEN" << std::endl;
					genU = parseWaveGenerator();
				}
				else {
					u = getNumericToken("pass.tcMod.scale: expecting numeric or wave value for U");
					
					std::stringstream ss;
					ss << u;
					ss >> uNum;
				}
				
				//std::cout << "current token = " << currentToken->second << std::endl;
				
				stepToken();
				if (currentToken->first == TGen::Q3MaterialTokenEndOfLine) {
					genV = genU;
					vNum = uNum;
				}
				else {
					if (currentToken->second == "sin") {
						genV = parseWaveGenerator();
					}
					else {
						v = getNumericToken("pass.tcMod.scale: expecting numeric or wave value for V");
						
						std::stringstream ss;
						ss << v;
						ss >> vNum;
					}
				}
				
				TGen::TextureCoordScale * scaler = new TGen::TextureCoordScale(genU, genV, centered);
				scaler->u = uNum;
				scaler->v = vNum;
				
				unit->addTexCoordTransformer(scaler);
			}
			else if (modType == "scroll") {
				std::string offU, offV;
				float U, V;
				
				offU = getNumericToken("texunit.tcMod.scroll: expecting numeric value for U");
				stepToken();
				offV = getNumericToken("texunit.tcMod.scroll: expecting numeric value for V");

				U = TGen::lexical_cast<float>(offU);
				V = TGen::lexical_cast<float>(offV);
				
				TGen::TextureCoordTranslate * translator = new TGen::TextureCoordTranslate((TGen::ScalarGenerator *)NULL, NULL, true);
				translator->u = U;
				translator->v = V;
				unit->addTexCoordTransformer(translator);
			}
			else if (modType == "rotate") {
				std::string rotSpeed;
				float rotSpeedNum;
				
				rotSpeed = getNumericToken("texunit.tcMod.rotate: expecting numeric value for rotation speed");
				rotSpeedNum = TGen::lexical_cast<float>(rotSpeed);
				
				TGen::TextureCoordRotate * rotator = new TGen::TextureCoordRotate(rotSpeedNum, true);
				unit->addTexCoordTransformer(rotator);
			}
			else {
				throw TGen::RuntimeException("Q3MaterialParser::ParsePassBlock", "tcMod type not supported '" + modType + "'!");
			}
		}
		else if (tokenInLower == "rgbgen") {
			stepToken();
			if (currentToken->second == "wave") {
				stepToken();
				pass->setColorGenerator(parseWaveGenerator());					
			}
			else {
				if (currentToken->second == "identity") {
					pass->setColor("1", "1", "1");
				}
				else if (currentToken->second == "vertex") {
					pass->setColorFromVertex();
				}
				else {
					std::string r, g, b;

					r = getNumericToken("pass.rgbgen: expecting numeric R value ");
					stepToken();
					g = getNumericToken("pass.rgbgen: expecting numeric G value ");
					stepToken();
					b = getNumericToken("pass.rgbgen: expecting numeric B value ");
					pass->setColor(r, g, b);
				}
			}
		}
		else if (tokenInLower == "map" || tokenInLower == "clampmap") {
			bool clampmap = currentToken->second == "clampmap";
			stepToken();
			std::string texName = getStringToken("pass.map: expecting string value for texture name");
			
			unit->setTextureName(texName);
			
			if (clampmap)
				unit->setWrap("clamp", "clamp");
		}
		else if (currentToken->first != TGen::Q3MaterialTokenEndOfLine) {
			throw TGen::RuntimeException("Q3MaterialParser::ParsePassBlock", "unexpected symbol: '" + currentToken->second + "'");
		}
		
		stepToken();
	}

	if (currentToken->first == TGen::Q3MaterialTokenBlockEnd)
		stepToken();
}

std::string TGen::Q3MaterialParser::getStringToken(const std::string & name, bool ignorelf, bool quote) {
	std::string ret;
	
	if (ignorelf) {
		while(currentToken->first == TGen::Q3MaterialTokenEndOfLine) {
			tokens.stepToken(currentToken, endIter);
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
			tokens.stepToken(currentToken, endIter);
		}
	}
	
	if (currentToken->first == TGen::TokenValueNumeric)
		ret = currentToken->second;
	else
		throw TGen::RuntimeException("MaterialParser::getNumericTokenk", name + ", not '" + currentToken->second + "' (") << currentToken->first << ")";
	
	return ret;
}

void TGen::Q3MaterialParser::stepOverLF() {
	do {
		stepToken();
	} while(currentToken->first == TGen::Q3MaterialTokenEndOfLine);	
}

void TGen::Q3MaterialParser::stepToken() {
	tokens.stepToken(currentToken, endIter);
}

TGen::WaveGenerator * TGen::Q3MaterialParser::parseWaveGenerator() {
	TGen::WaveGenerator * ret = NULL;
	
	std::string type, base, amplitude, phase, frequency;
	type = getStringToken("wave: expecting string value for wave type", true);
	stepToken();
	base = getNumericToken("wave: expecting numeric value for base", true);
	stepToken();
	amplitude = getNumericToken("wave: expecting numeric value for amplitude", true);
	stepToken();
	phase = getNumericToken("wave: expecting numeric value for phase", true);
	stepToken();
	frequency = getNumericToken("wave: expecting numeric value for frequency", true);
	
	std::stringstream ss;
	scalar baseNumber, amplitudeNumber, phaseNumber, frequencyNumber;
	ss << base << " " << amplitude << " " << phase << " " << frequency;
	ss >> baseNumber >> amplitudeNumber >> phaseNumber >> frequencyNumber;
	
	if (type == "sine" || type == "sin")
		ret = new TGen::WaveGenerator(TGen::WaveSine, baseNumber, amplitudeNumber, phaseNumber, frequencyNumber);
	else if (type == "square")
		ret = new TGen::WaveGenerator(TGen::WaveSquare, baseNumber, amplitudeNumber, phaseNumber, frequencyNumber);		
	else if (type == "sawtooth" || type == "saw")
		ret = new TGen::WaveGenerator(TGen::WaveSawtooth, baseNumber, amplitudeNumber, phaseNumber, frequencyNumber);	
	else if (type == "inversesaw" || type == "inversesawtooth")
		ret = new TGen::WaveGenerator(TGen::WaveInverseSawtooth, baseNumber, amplitudeNumber, phaseNumber, frequencyNumber);
	else if (type == "triangle")
		ret = new TGen::WaveGenerator(TGen::WaveTriangle, baseNumber, amplitudeNumber, phaseNumber, frequencyNumber);		
	else
		throw TGen::RuntimeException("Q3MaterialParser::ParseWaveGenerator", "invalid wave type: '" + currentToken->second + "'!");
	
	return ret;
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