/*
 *  materialparser.cpp
 *  TGen Graphics
 *
 *  Created by Peter Backman on 7/4/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "materialparser.h"
#include "material.h"
#include "technique.h"
#include "pass.h"
#include "generator.h"
#include <iostream>

TGen::MaterialParser::MaterialParser() {}
TGen::MaterialParser::~MaterialParser() {}

void TGen::MaterialParser::Parse(const char * code, std::list<TGen::Material *> & materials) {
	TGen::MaterialTokenizer tokenizer;	
	tokenizer.TokenizeString(code, tokens, false);
	
	currentToken = tokens.getFirstToken();
	endIter = tokens.getEndToken();
	//currentMaterial = NULL;
	currentTechnique = NULL;
	currentSpecialization = NULL;
	//currentLod = NULL;
	
	ParseGlobalBlock();
	
	
	MaterialMap::iterator iter = this->materials.begin();
	for (; iter != this->materials.end(); ++iter) {
		materials.push_back(iter->second);
	}	
}

void TGen::MaterialParser::ParseGlobalBlock() {
	while (currentToken != endIter) {
		if (currentToken->first == TGen::MaterialTokenMaterial) {
			std::string materialName, specialization, technique;
			
			tokens.NextToken(currentToken, endIter);
			
			if (currentToken->first == TGen::TokenValueString)
				materialName = currentToken->second;
			else
				throw TGen::RuntimeException("MaterialParser::ParseGlobalBlock", "material: expecting string value for name, not '" + currentToken->second + "'!");
			
			tokens.NextToken(currentToken, endIter);
			
			if (currentToken->first == TGen::TokenValueString)
				specialization = currentToken->second;
			else
				throw TGen::RuntimeException("MaterialParser::ParseGlobalBlock", "material: expecting string value for specialization!");
			
			tokens.NextToken(currentToken, endIter);
			
			if (currentToken->first == TGen::TokenValueNumeric)
				technique = currentToken->second;
			else
				throw TGen::RuntimeException("MaterialParser::ParseGlobalBlock", "material: expecting numeric value for technique!");
			
			int techniqueNumber = 0;
			std::stringstream ss;
			ss << technique;
			ss >> techniqueNumber;
			
			do {
				tokens.NextToken(currentToken, endIter);
			} while(currentToken->first == TGen::MaterialTokenEndOfLine);
			
			if (currentToken->first != TGen::MaterialTokenBlockStart) 
				throw TGen::RuntimeException("MaterialParser::ParseGlobalBlock", "material: expecting block start, not '" + currentToken->second + "'!");
			
			std::cout << "Creating new material: name: '" << materialName << "' specialization: '" << specialization << "' technique: " << techniqueNumber <<  std::endl;

			
			TGen::Material * newMaterial = getMaterial(materialName);
			TGen::TechniqueList * newTechniqueList = new TGen::TechniqueList;
			TGen::Technique * newTechnique = new TGen::Technique;
			
			newTechniqueList->setTechnique(newTechnique, techniqueNumber);
			newMaterial->setSpecialization(specialization, newTechniqueList);
			
			//currentMaterial = newMaterial;
			currentSpecialization = newTechniqueList;
			currentTechnique = newTechnique;
			tokens.NextToken(currentToken, endIter);

			std::cout << "entering material block" << std::endl;
			ParseMaterialBlock(newMaterial);
			std::cout << "left material block" << std::endl;
		
			continue;
		}
		else if (currentToken->first == TGen::MaterialTokenParameters) {
			std::string materialName;
			tokens.NextToken(currentToken, endIter);
			
			if (currentToken->first == TGen::TokenValueString)
				materialName = currentToken->second;
			else
				throw TGen::RuntimeException("MaterialParser::ParseGlobalBlock", "params: expecting string value for name, not '" + currentToken->second + "'!");			
		
			do {
				tokens.NextToken(currentToken, endIter);
			} while(currentToken->first == TGen::MaterialTokenEndOfLine);
			
			if (currentToken->first != TGen::MaterialTokenBlockStart) 
				throw TGen::RuntimeException("MaterialParser::ParseGlobalBlock", "params: expecting block start, not '" + currentToken->second + "'!");
			
			std::cout << "entering params block for material '" << materialName << "'" << std::endl;
			//currentMaterial = getMaterial(materialName);
			tokens.NextToken(currentToken, endIter);

			ParseParamsBlock(getMaterial(materialName));
			std::cout << "leaving params block" << std::endl;
			
			continue;
		}
		else if (currentToken->first != TGen::MaterialTokenEndOfLine) {
			throw TGen::RuntimeException("MaterialParser::ParseGlobalBlock", "expecting 'material' or 'params', not '" + currentToken->second + "'!");
		}
		
		tokens.NextToken(currentToken, endIter);
	}	
	
}

void TGen::MaterialParser::ParseParamsBlock(TGen::Material * material) {
	while (currentToken != endIter && currentToken->first != TGen::MaterialTokenBlockEnd) {
		if (currentToken->first == TGen::TokenValueString || currentToken->first == TGen::TokenQuote) {
			std::string paramName = currentToken->second;
			std::list<std::string> parameters;
			std::cout << "param name: '" << paramName << "' parameter: ";
			
			do {
				tokens.NextToken(currentToken, endIter);
				
				if (currentToken->first == TGen::TokenValueNumeric || currentToken->first == TGen::TokenValueString || currentToken->first == TGen::TokenQuote) {
					parameters.push_back(currentToken->second);
					std::cout << "'" << currentToken->second << "' ";
				}
				
			} while (currentToken->first != TGen::MaterialTokenEndOfLine && currentToken->first != TGen::MaterialTokenBlockEnd);
			
			std::cout << std::endl;
		}
		else if (currentToken->first != TGen::MaterialTokenEndOfLine) {
			throw TGen::RuntimeException("MaterialParser::ParseParamsBlock", "expecting string parameter name, not '" + currentToken->second + "'!");
		}
		
		tokens.NextToken(currentToken, endIter);
	}
	
	if (currentToken->first == TGen::MaterialTokenBlockEnd)
		tokens.NextToken(currentToken, endIter);
}

void TGen::MaterialParser::ParseMaterialBlock(TGen::Material * material) {
	while (currentToken != endIter && currentToken->first != TGen::MaterialTokenBlockEnd) {
		if (currentToken->first == TGen::MaterialTokenLod) {
			std::string lodValue;
			tokens.NextToken(currentToken, endIter);
			
			if (currentToken->first == TGen::TokenValueNumeric)
				lodValue = currentToken->second;
			else
				throw TGen::RuntimeException("MaterialParser::ParseMaterialBlock", "lod: expecting numeric value for lod, not '" + currentToken->second + "'!");
			
			do {
				tokens.NextToken(currentToken, endIter);
			} while(currentToken->first == TGen::MaterialTokenEndOfLine);
			
			if (currentToken->first != TGen::MaterialTokenBlockStart) 
				throw TGen::RuntimeException("MaterialParser::ParseMaterialBlock", "lod: expecting block start, not '" + currentToken->second + "'!");
			
			int lodNumber = 0;
			std::stringstream ss;
			ss << lodValue;
			ss >> lodNumber;
			
			TGen::PassList * newLod = new TGen::PassList;
			
			if (!currentTechnique)
				throw TGen::RuntimeException("MaterialParser::ParseMaterialBlock", "lod: no current technique!!");
			
			currentTechnique->setPassList(newLod, lodNumber);
			//currentLod = newLod;
			
			tokens.NextToken(currentToken, endIter);
			
			std::cout << "entering lod block" << std::endl;
			ParseLodBlock(newLod);
			std::cout << "left lod block" << std::endl;
			
		}
		else if (currentToken->first != TGen::MaterialTokenEndOfLine) {
			throw TGen::RuntimeException("MaterialParser::ParseMaterialBlock", "not expecting '" + currentToken->second + "'!");			
		}
		
		tokens.NextToken(currentToken, endIter);		
	}
	
	if (currentToken->first == TGen::MaterialTokenBlockEnd)
		tokens.NextToken(currentToken, endIter);
}

void TGen::MaterialParser::ParseLodBlock(TGen::PassList * lod) {
	while (currentToken != endIter && currentToken->first != TGen::MaterialTokenBlockEnd) {
		if (currentToken->first == TGen::MaterialTokenPass) {
			std::string shaderName;
			
			tokens.NextToken(currentToken, endIter);
			
			shaderName = getStringToken("pass: expecting string value for shader", true);
			
			do {
				tokens.NextToken(currentToken, endIter);
			} while(currentToken->first == TGen::MaterialTokenEndOfLine);
			
			if (currentToken->first != TGen::MaterialTokenBlockStart) 
				throw TGen::RuntimeException("MaterialParser::ParseLodBlock", "pass: expecting block start, not '" + currentToken->second + "'!");
			
			std::cout << "creating new '" << shaderName << "' pass" << std::endl;
			
			TGen::Pass * newPass = new TGen::Pass;
			newPass->setShader(shaderName);
			lod->addPass(newPass);
			
			tokens.NextToken(currentToken, endIter);
			
			std::cout << "entering pass block" << std::endl;
			ParsePassBlock(newPass);
			std::cout << "left pass block" << std::endl;		
		}
		else if (currentToken->first != TGen::MaterialTokenEndOfLine) {
			throw TGen::RuntimeException("MaterialParser::ParseLodBlock", "not expecting '" + currentToken->second + "'!");			
		}
		
		tokens.NextToken(currentToken, endIter);		
	}
	
	if (currentToken->first == TGen::MaterialTokenBlockEnd)
		tokens.NextToken(currentToken, endIter);
}

void TGen::MaterialParser::ParsePassBlock(TGen::Pass * pass) {
	while (currentToken != endIter && currentToken->first != TGen::MaterialTokenBlockEnd) {
		if (currentToken->first == TGen::TokenValueString) {
			if (currentToken->second == "texunit") {
				std::string textureUnit, textureName;
				
				StepToken();
				textureUnit = getNumericToken("texunit: expecting numeric value for texture unit");
				StepToken();
				textureName = getStringToken("texunit: expecting string value for texture name");
				
				do {
					StepToken();
				} while(currentToken->first == TGen::MaterialTokenEndOfLine);
				
				if (currentToken->first != TGen::MaterialTokenBlockStart) 
					throw TGen::RuntimeException("MaterialParser::ParsePassBlock", "texunit: expecting block start, not '" + currentToken->second + "'!");

				int textureUnitNumber = -1;
				std::stringstream ss;
				ss << textureUnit;
				ss >> textureUnitNumber;
				
				TGen::PassTextureUnit * newTextureUnit = new TGen::PassTextureUnit(textureUnitNumber, textureName);
				
				StepToken();
				std::cout << "entering texunit " << textureUnit << " name: " << textureName << std::endl;
				ParseTexunitBlock(newTextureUnit);
				std::cout << "leaving texunit" << std::endl;
				
				pass->AddTextureUnit(newTextureUnit);
			}
			else if (currentToken->second == "color") {
				std::string r, g, b;
				
				StepToken();
				if (currentToken->second == "wave") {
					StepToken();
					pass->setColorGenerator(ParseWaveGenerator());					
				}
				else {
					r = getNumericToken("pass.color: expecting numeric R value ");
					StepToken();
					g = getNumericToken("pass.color: expecting numeric G value ");
					StepToken();
					b = getNumericToken("pass.color: expecting numeric B value ");
				
					pass->setColor(r, g, b);
				}
			}
			else if (currentToken->second == "alpha") {
				std::string a;
				
				StepToken();
				if (currentToken->second == "wave") {
					StepToken();
					pass->setAlphaGenerator(ParseWaveGenerator());					
				}
				else {
					a = getNumericToken("pass.alpha: expecting numeric A value ");
					
					pass->setAlpha(a);
				}
			}
			else if (currentToken->second == "noDepthWrite") {
				pass->setNoDepthWrite();
			}
			else if (currentToken->second == "front") {
				std::string polyMode;
				
				StepToken();
				polyMode = getStringToken("pass.front: expecting string value for mode");
				
				pass->setFrontMode(polyMode);
			}
			else if (currentToken->second == "back") {
				std::string polyMode;
				
				StepToken();
				polyMode = getStringToken("pass.back: expecting string value for mode");
				
				pass->setBackMode(polyMode);
			}
			else if (currentToken->second == "depthPass") {
				StepToken();
				
				pass->setDepthFunc(getStringToken("pass.depthPass: expecting string value for compare func"));
			}
			else if (currentToken->second == "blendFunc") {
				StepToken();
				std::string source, dest;
				
				source = getStringToken("pass.blend: expecting string value for func");
				
				StepToken();
				if (currentToken->first == TGen::TokenValueString || currentToken->first == TGen::TokenQuote)
					dest = currentToken->second;
				
				pass->setBlendFunc(source, dest);
			}
			else {
				throw TGen::RuntimeException("MaterialParser::ParsePassBlock", "not expecting '" + currentToken->second + "'!");							
			}
		}
		else if (currentToken->first != TGen::MaterialTokenEndOfLine) {
			throw TGen::RuntimeException("MaterialParser::ParsePassBlock", "not expecting '" + currentToken->second + "'!");			
		}
		
		tokens.NextToken(currentToken, endIter);		
	}
	
	if (currentToken->first == TGen::MaterialTokenBlockEnd)
		tokens.NextToken(currentToken, endIter);	
}

void TGen::MaterialParser::ParseTexunitBlock(TGen::PassTextureUnit * unit) {
	while (currentToken != endIter && currentToken->first != TGen::MaterialTokenBlockEnd) {
		if (currentToken->first == TGen::TokenValueString) {
			if (currentToken->second == "texCoordGen") {
				std::string genU, genV;

				StepToken();
				genU = getStringToken("texunit.texCoordGen: expecting string value for U");
				StepToken();
				genV = getStringToken("texunit.texCoordGen: expecting string value for V");
				
				unit->setTexCoordGen(genU, genV);
			}
			else if (currentToken->second == "translate" || currentToken->second == "scroll") {
				std::string offU, offV;
				float u = 0.0f, v = 0.0f;
				bool scroll = currentToken->second == "scroll";
				
				TGen::ScalarGenerator * genU = NULL, * genV = NULL;

				StepToken();
				if (currentToken->second == "wave") {
					StepToken();
					genU = ParseWaveGenerator();
				}
				else {
					offU = getNumericToken("texunit.translate: expecting numeric or wave value for U offset");
				
					std::stringstream ss;
					ss << offU;
					ss >> u;
				}
				
				StepToken();
				if (currentToken->second == "wave") {
					StepToken();
					genV = ParseWaveGenerator();
				}
				else {
					offV = getNumericToken("texunit.translate: expecting numeric or wave value for V offset");

					std::stringstream ss;
					ss << offV;
					ss >> v;
				}
				
				TGen::TextureCoordTranslate * translator = new TGen::TextureCoordTranslate(genU, genV, scroll);
				translator->u = u;
				translator->v = v;
				
				unit->AddTexCoordTransformer(translator);
			}
			else if (currentToken->second == "scale" || currentToken->second == "stretch") {
				std::string u, v;
				float uNum = 0.0f, vNum = 0.0f;
				bool centered = currentToken->second == "stretch";
				
				TGen::ScalarGenerator * genU = NULL, * genV = NULL;
				
				StepToken();
				if (currentToken->second == "wave") {
					StepToken();
					genU = ParseWaveGenerator();
				}
				else {
					u = getNumericToken("texunit.scale: expecting numeric or wave value for U");
					
					std::stringstream ss;
					ss << u;
					ss >> uNum;
				}
				
				StepToken();
				if (currentToken->first == TGen::MaterialTokenEndOfLine) {
					genV = genU;
					vNum = uNum;
				}
				else {
					if (currentToken->second == "wave") {
						StepToken();
						genV = ParseWaveGenerator();
					}
					else {
						v = getNumericToken("texunit.scale: expecting numeric or wave value for V");
						
						std::stringstream ss;
						ss << v;
						ss >> vNum;
					}
				}
				
				
				TGen::TextureCoordScale * scaler = new TGen::TextureCoordScale(genU, genV, centered);
				scaler->u = uNum;
				scaler->v = vNum;
				
				unit->AddTexCoordTransformer(scaler);
			}
			else if (currentToken->second == "rotate" || currentToken->second == "spin") {
				float u = 0.0f;
				bool centered = currentToken->second == "spin";
				
				TGen::ScalarGenerator * genRot = NULL;
				StepToken();
				if (currentToken->second == "wave") {
					StepToken();
					genRot = ParseWaveGenerator();
				}
				else {
					std::string uName = getNumericToken("texunit.rotate: expecting numeric or wave value for U");
					
					std::stringstream ss;
					ss << uName;
					ss >> u;
				}
				
				TGen::TextureCoordRotate * rotator = new TGen::TextureCoordRotate(genRot, centered);
				rotator->speed = u;
				
				unit->AddTexCoordTransformer(rotator);				
			}
			else if (currentToken->second == "sampler") {
				std::string samplerName;
				
				StepToken();
				samplerName = getStringToken("texunit.sampler: expecting string value for sampler name");
				
				unit->setSampler(samplerName);
			}
			else {
				throw TGen::RuntimeException("MaterialParser::ParseTexunitBlock", "not expecting '" + currentToken->second + "'!");
			}
		}
		else if (currentToken->first != TGen::MaterialTokenEndOfLine) {
			throw TGen::RuntimeException("MaterialParser::ParseTexunitBlock", "not expecting '" + currentToken->second + "'!");			
		}
		
		tokens.NextToken(currentToken, endIter);		
	}
	
	if (currentToken->first == TGen::MaterialTokenBlockEnd)
		tokens.NextToken(currentToken, endIter);	
}

TGen::Material * TGen::MaterialParser::getMaterial(const std::string & name) {
	MaterialMap::iterator iter = materials.find(name);
	if (iter == materials.end()) {
		TGen::Material * newMaterial = new TGen::Material(name);
		materials[name] = newMaterial;
		
		return newMaterial;
	}
	
	return iter->second;
}

TGen::MaterialTokenizer::MaterialTokenizer() {
	autoAddTilNextIgnore = true;
	
	ignores.push_back(" ");
	ignores.push_back("\t");
	ignores.push_back("\r");
//	ignores.push_back("\n");

	quotes.push_back(StringPair("\"", "\""));
	comments.push_back(StringPair("//", "\n"));
	comments.push_back(StringPair("/*", "*/"));
		
	tokens["{"] = TGen::MaterialTokenBlockStart;
	tokens["}"] = TGen::MaterialTokenBlockEnd;
	tokens["material"] = TGen::MaterialTokenMaterial;
	tokens["pass"] = TGen::MaterialTokenPass;
	tokens["lod"] = TGen::MaterialTokenLod;
	tokens["params"] = TGen::MaterialTokenParameters;	
	tokens["\n"] = TGen::MaterialTokenEndOfLine;		// we need these where parameters are unlimited
}

int TGen::MaterialTokenizer::getSpecialToken(char * text, TGen::TokenStream & stream) {
	return 0;
}

std::string TGen::MaterialParser::getStringToken(const std::string & name, bool ignorelf, bool quote) {
	std::string ret;
	
	if (ignorelf) {
		while(currentToken->first == TGen::MaterialTokenEndOfLine) {
			tokens.NextToken(currentToken, endIter);
		}
	}
	
	if (currentToken->first == TGen::TokenValueString || currentToken->first == TGen::TokenQuote)
		ret = currentToken->second;
	else
		throw TGen::RuntimeException("MaterialParser::getStringToken", name + ", not '" + currentToken->second + "' (") << currentToken->first << ")";
	
	return ret;
}

std::string TGen::MaterialParser::getNumericToken(const std::string & name, bool ignorelf, bool quote) {
	std::string ret;
	
	if (ignorelf) {
		while(currentToken->first == TGen::MaterialTokenEndOfLine) {
			tokens.NextToken(currentToken, endIter);
		}
	}
	
	if (currentToken->first == TGen::TokenValueNumeric)
		ret = currentToken->second;
	else
		throw TGen::RuntimeException("MaterialParser::getNumericTokenk", name + ", not '" + currentToken->second + "' (") << currentToken->first << ")";
	
	return ret;
}

void TGen::MaterialParser::StepToken() {
	tokens.NextToken(currentToken, endIter);
}

TGen::WaveGenerator * TGen::MaterialParser::ParseWaveGenerator() {
	TGen::WaveGenerator * ret = NULL;

	std::string type, base, amplitude, phase, frequency;
	type = getStringToken("wave: expecting string value for wave type", true);
	StepToken();
	base = getNumericToken("wave: expecting numeric value for base", true);
	StepToken();
	amplitude = getNumericToken("wave: expecting numeric value for amplitude", true);
	StepToken();
	phase = getNumericToken("wave: expecting numeric value for phase", true);
	StepToken();
	frequency = getNumericToken("wave: expecting numeric value for frequency", true);
	
	std::stringstream ss;
	scalar baseNumber, amplitudeNumber, phaseNumber, frequencyNumber;
	ss << base << " " << amplitude << " " << phase << " " << frequency;
	ss >> baseNumber >> amplitudeNumber >> phaseNumber >> frequencyNumber;
	
	if (type == "sine" || type == "sin")
		ret = new TGen::SineWaveGenerator(baseNumber, amplitudeNumber, phaseNumber, frequencyNumber);
	else if (type == "square")
		ret = new TGen::SquareWaveGenerator(baseNumber, amplitudeNumber, phaseNumber, frequencyNumber);		
	else if (type == "sawtooth" || type == "saw")
		ret = new TGen::SawtoothWaveGenerator(baseNumber, amplitudeNumber, phaseNumber, frequencyNumber);		
	else
		throw TGen::RuntimeException("MaterialParser::ParseWaveGenerator", "invalid wave type: '" + currentToken->second + "'!");
	
	return ret;
}
