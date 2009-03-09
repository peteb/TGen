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
#include "techniquelist.h"
#include "pass.h"
#include "generator.h"
#include "passlist.h"
#include "shadervarupdater.h"
#include "texturetransformer.h"
#include "passtextureunit.h"
#include "typesconverter.h"
#include <iostream>

TGen::MaterialParser::MaterialParser() {}
TGen::MaterialParser::~MaterialParser() {}

void TGen::MaterialParser::parse(const char * code, std::list<TGen::Material *> & materials) {
	TGen::MaterialTokenizer tokenizer;	
	tokenizer.tokenizeString(code, tokens, false);

	
	currentToken = tokens.getFirstToken();
	endIter = tokens.getEndToken();
	//currentMaterial = NULL;
	currentTechnique = NULL;
	currentSpecialization = NULL;
	//currentLod = NULL;
	
	parseGlobalBlock();
	
	
	MaterialMap::iterator iter = this->materials.begin();
	for (; iter != this->materials.end(); ++iter) {
		materials.push_back(iter->second);
	}	
}

void TGen::MaterialParser::parseGlobalBlock() {
	while (currentToken != endIter) {
		if (currentToken->first == TGen::MaterialTokenMaterial) {
			std::string materialName, specialization, technique;
			
			tokens.stepToken(currentToken, endIter);
			
			if (currentToken->first == TGen::TokenValueString)
				materialName = currentToken->second;
			else
				throw TGen::RuntimeException("MaterialParser::ParseGlobalBlock", "material: expecting string value for name, not '" + currentToken->second + "'!");
			
			tokens.stepToken(currentToken, endIter);
			
			if (currentToken->first == TGen::TokenValueString)
				specialization = currentToken->second;
			else
				throw TGen::RuntimeException("MaterialParser::ParseGlobalBlock", "material: expecting string value for specialization!");
			
			tokens.stepToken(currentToken, endIter);
			
			if (currentToken->first == TGen::TokenValueNumeric)
				technique = currentToken->second;
			else
				throw TGen::RuntimeException("MaterialParser::ParseGlobalBlock", "material: expecting numeric value for technique, not ") << currentToken->second;
			
			int techniqueNumber = 0;
			std::stringstream ss;
			ss << technique;
			ss >> techniqueNumber;
			
			do {
				tokens.stepToken(currentToken, endIter);
			} while(currentToken->first == TGen::MaterialTokenEndOfLine);
			
			if (currentToken->first != TGen::MaterialTokenBlockStart) 
				throw TGen::RuntimeException("MaterialParser::ParseGlobalBlock", "material: expecting block start, not '" + currentToken->second + "'!");
			
			#ifdef _MATPARSER_VERBOSE
			std::cout << "Creating new material: name: '" << materialName << "' specialization: '" << specialization << "' technique: " << techniqueNumber <<  std::endl;
			#endif
			
			
			TGen::Material * newMaterial = getMaterial(materialName);
			TGen::TechniqueList * newTechniqueList = newMaterial->getTechniqueList(specialization);
			
			if (!newTechniqueList) {
				newTechniqueList = new TGen::TechniqueList(specialization);
				newMaterial->setSpecialization(specialization, newTechniqueList);
			}
			
			TGen::Technique * newTechnique = new TGen::Technique;
			
			newTechniqueList->setTechnique(newTechnique, techniqueNumber);
			
			//currentMaterial = newMaterial;
			currentSpecialization = newTechniqueList;
			currentTechnique = newTechnique;
			tokens.stepToken(currentToken, endIter);

			#ifdef _MATPARSER_VERBOSE
			std::cout << "entering material block" << std::endl;
			#endif
			
			parseMaterialBlock(newMaterial);
			
			#ifdef _MATPARSER_VERBOSE
			std::cout << "left material block" << std::endl;
			#endif
			
			continue;
		}
		else if (currentToken->first == TGen::MaterialTokenParameters) {
			std::string materialName;
			stepToken();
			
			if (currentToken->first == TGen::TokenValueString)
				materialName = currentToken->second;
			else
				throw TGen::RuntimeException("MaterialParser::ParseGlobalBlock", "params: expecting string value for name, not '" + currentToken->second + "'!");			
		
			do {
				stepToken();
			} while(currentToken->first == TGen::MaterialTokenEndOfLine);
			
			if (currentToken->first != TGen::MaterialTokenBlockStart) 
				throw TGen::RuntimeException("MaterialParser::ParseGlobalBlock", "params: expecting block start, not '" + currentToken->second + "'!");
			
			#ifdef _MATPARSER_VERBOSE
			std::cout << "entering params block for material '" << materialName << "'" << std::endl;
			#endif
			
			stepToken();
			parseParamsBlock(getMaterial(materialName));
			
			#ifdef _MATPARSER_VERBOSE
			std::cout << "leaving params block" << std::endl;
			#endif
			
			continue;
		}
		else if (currentToken->first != TGen::MaterialTokenEndOfLine) {
			throw TGen::RuntimeException("MaterialParser::ParseGlobalBlock", "expecting 'material' or 'params', not '" + currentToken->second + "'!");
		}
		
		tokens.stepToken(currentToken, endIter);
	}	
	
}

void TGen::MaterialParser::parseParamsBlock(TGen::Material * material) {
	while (currentToken != endIter && currentToken->first != TGen::MaterialTokenBlockEnd) {
		if (currentToken->first == TGen::TokenValueString || currentToken->first == TGen::TokenQuote) {
			std::string paramName = currentToken->second;
			std::vector<std::string> parameters;
			
			#ifdef _MATPARSER_VERBOSE
			std::cout << "param name: '" << paramName << "' parameter: ";
			#endif
			
			do {
				stepToken();
				
				if (currentToken->first == TGen::TokenValueNumeric || currentToken->first == TGen::TokenValueString || currentToken->first == TGen::TokenQuote) {
					parameters.push_back(currentToken->second);
					
					#ifdef _MATPARSER_VERBOSE
					std::cout << "'" << currentToken->second << "' ";
					#endif
				}
				
			} while (currentToken->first != TGen::MaterialTokenEndOfLine && currentToken->first != TGen::MaterialTokenBlockEnd);
			
			#ifdef _MATPARSER_VERBOSE
			std::cout << std::endl;
			#endif
			material->setParameter(paramName, parameters);
		}
		else if (currentToken->first != TGen::MaterialTokenEndOfLine) {
			throw TGen::RuntimeException("MaterialParser::ParseParamsBlock", "expecting string parameter name, not '" + currentToken->second + "'!");
		}
		
		stepToken();
	}
	
	if (currentToken->first == TGen::MaterialTokenBlockEnd)
		stepToken();
}

void TGen::MaterialParser::parseMaterialBlock(TGen::Material * material) {
	while (currentToken != endIter && currentToken->first != TGen::MaterialTokenBlockEnd) {
		if (currentToken->first == TGen::MaterialTokenLod) {
			std::string lodValue;
			stepToken();
			
			if (currentToken->first == TGen::TokenValueNumeric)
				lodValue = currentToken->second;
			else
				throw TGen::RuntimeException("MaterialParser::ParseMaterialBlock", "lod: expecting numeric value for lod, not '" + currentToken->second + "'!");
			
			do {
				tokens.stepToken(currentToken, endIter);
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
			
			stepToken();
			
			#ifdef _MATPARSER_VERBOSE
			std::cout << "entering lod block" << std::endl;
			#endif
			
			parseLodBlock(newLod, material);
			
			#ifdef _MATPARSER_VERBOSE
			std::cout << "left lod block" << std::endl;
			#endif
			
		}
		else if (currentToken->first != TGen::MaterialTokenEndOfLine) {
			throw TGen::RuntimeException("MaterialParser::ParseMaterialBlock", "not expecting '" + currentToken->second + "'!");			
		}
		
		stepToken();		
	}
	
	if (currentToken->first == TGen::MaterialTokenBlockEnd)
		stepToken();
}

void TGen::MaterialParser::parseLodBlock(TGen::PassList * lod, TGen::Material * material) {
	while (currentToken != endIter && currentToken->first != TGen::MaterialTokenBlockEnd) {
		if (currentToken->first == TGen::MaterialTokenPass) {
			std::string shaderName;
			
			stepToken();
			shaderName = getStringToken("pass: expecting string value for shader", true);
			
			do {
				stepToken();
			} while(currentToken->first == TGen::MaterialTokenEndOfLine);
			
			if (currentToken->first != TGen::MaterialTokenBlockStart) 
				throw TGen::RuntimeException("MaterialParser::ParseLodBlock", "pass: expecting block start, not '" + currentToken->second + "'!");
			
			#ifdef _MATPARSER_VERBOSE
			std::cout << "creating new '" << shaderName << "' pass" << std::endl;
			#endif
			
			
			TGen::Pass * newPass = new TGen::Pass;
			
			
			bool zeroSet = false;
			
			if (shaderName != "fixed") {
				for (ShaderPermutationList::const_iterator iter = shaderPermutations.begin(); iter != shaderPermutations.end(); ++iter) {
					std::string fixedShader = shaderName;
					
					if (fixedShader.find(":") != std::string::npos)
						fixedShader += "," + iter->first;
					else
						fixedShader += ":" + iter->first;
						
					newPass->setShader(fixedShader, iter->second);
					
					if (iter->second == 0)
						zeroSet = true;
				}
			}

			if (!zeroSet)
				newPass->setShader(shaderName, 0);

			
			lod->addPass(newPass);
			
			stepToken();
			
			#ifdef _MATPARSER_VERBOSE
			std::cout << "entering pass block" << std::endl;
			#endif
			
			parsePassBlock(newPass, lod, material);
			
			#ifdef _MATPARSER_VERBOSE
			std::cout << "left pass block" << std::endl;
			#endif
		}
		else if (currentToken->first != TGen::MaterialTokenEndOfLine) {
			throw TGen::RuntimeException("MaterialParser::ParseLodBlock", "not expecting '" + currentToken->second + "'!");			
		}
		
		tokens.stepToken(currentToken, endIter);		
	}
	
	if (currentToken->first == TGen::MaterialTokenBlockEnd)
		tokens.stepToken(currentToken, endIter);
}

void TGen::MaterialParser::parsePassBlock(TGen::Pass * pass, TGen::PassList * lod, TGen::Material * material) {
	while (currentToken != endIter && currentToken->first != TGen::MaterialTokenBlockEnd) {
		if (currentToken->first == TGen::TokenValueString) {
			if (currentToken->second == "texunit") {
				std::string textureUnit, textureName;
				
				stepToken();
				textureUnit = getNumericToken("texunit: expecting numeric value for texture unit");
				stepToken();
				textureName = getStringToken("texunit: expecting string value for texture name");
				
				do {
					stepToken();
				} while(currentToken->first == TGen::MaterialTokenEndOfLine);
				
				if (currentToken->first != TGen::MaterialTokenBlockStart) 
					throw TGen::RuntimeException("MaterialParser::ParsePassBlock", "texunit: expecting block start, not '" + currentToken->second + "'!");

				int textureUnitNumber = -1;
				std::stringstream ss;
				ss << textureUnit;
				ss >> textureUnitNumber;
				
				TGen::PassTextureUnit * newTextureUnit = new TGen::PassTextureUnit(textureUnitNumber, textureName);
				
				stepToken();
				
				#ifdef _MATPARSER_VERBOSE
				std::cout << "entering texunit " << textureUnit << " name: " << textureName << std::endl;
				#endif
				
				parseTexunitBlock(newTextureUnit, material);
				
				#ifdef _MATPARSER_VERBOSE
				std::cout << "leaving texunit" << std::endl;
				#endif
				
				pass->addTextureUnit(newTextureUnit);
			}
			else if (currentToken->second == "map") {
				std::string textureSampler, textureName;
				
				stepToken();
				textureSampler = getStringToken("map: expecting string value for texture sampler");
				stepToken();
				textureName = getStringToken("map: expecting string value for texture name");
				
				std::cout << "sampler: " << textureSampler << " TEXNAME: " << textureName << std::endl;
				
				do {
					stepToken();
				} while(currentToken->first == TGen::MaterialTokenEndOfLine);
				
				TGen::PassTextureUnit * newTextureUnit = new TGen::PassTextureUnit(pass->getNumTextureUnits(), textureName);
			
				newTextureUnit->setSampler(textureSampler);
				
				if (currentToken->first == TGen::MaterialTokenBlockStart) {
					stepToken();
					parseTexunitBlock(newTextureUnit, material);
				}
				
				pass->addTextureUnit(newTextureUnit);
				continue;
			}
			else if (currentToken->second == "color") {
				std::string r, g, b;
				
				stepToken();
				if (currentToken->second == "wave") {
					stepToken();
					pass->setColorGenerator(parseWaveGenerator());					
				}
				else if (currentToken->second == "vertex") {
					stepToken();
					pass->setColorFromVertex();
				}
				else {				
					pass->setColor(parseColor());
				}
			}
			else if (currentToken->second == "alpha") {
				std::string a;
				lod->setSortLevel(TGen::MaterialSortTransparent);
				
				stepToken();
				if (currentToken->second == "wave") {
					stepToken();
					pass->setAlphaGenerator(parseWaveGenerator());					
				}
				else {
					a = getNumericToken("pass.alpha: expecting numeric A value ");
					
					pass->setAlpha(TGen::lexical_cast<scalar>(a));
				}
			}
			else if (currentToken->second == "lightDiffuse") {
				stepToken();
				pass->setLightDiffuse(parseColor());
			}
			else if (currentToken->second == "lightSpecular") {
				stepToken();
				pass->setLightSpecular(parseColor());
			}
			else if (currentToken->second == "lightShininess") {
				stepToken();
				pass->setLightShininess(TGen::lexical_cast<scalar>(getNumericToken("pass.lightShininess: expecting numeric value")));
			}
			else if (currentToken->second == "bind") {
				std::string varName, varId;
				stepToken();
				varName = getStringToken("pass.bind: expecting string value for variable name to bind");
				stepToken();
				varId = getStringToken("pass.bind: expecting string value to bind to");
				
				pass->addShaderVariable(varName, varId);
				stepToken();
			}
			else if (currentToken->second == "param") {
				std::string varName, varType, varValue;
				stepToken();
				varName = getStringToken("pass.param: expecting string value for variable name");
				stepToken();
				varType = getStringToken("pass.param: expecting string value for variable type");
				stepToken();
				//varValue = getStringToken("pass.param: expecting string value for variable value");
				
				// TODO: VarTypeParser
				TGen::ShaderVarType type;
				if (varType == "int")
					type = TGen::ShaderVarInt;
				else if (varType == "float")
					type = TGen::ShaderVarFloat;
				else if (varType == "float2")
					type = TGen::ShaderVarFloat2;
				else if (varType == "float3")
					type = TGen::ShaderVarFloat3;
				else if (varType == "float4")
					type = TGen::ShaderVarFloat4;
				
				
				TGen::ShaderUpdater * newUpdater = new TGen::ShaderUpdater(varName, type, TGen::lexical_cast<float>(getNumericToken("blaba")));
				pass->addShaderUpdater(newUpdater);
				
				stepToken();
			}
			else if (currentToken->second == "noDepthWrite") {
				pass->setNoDepthWrite();
			}
			else if (currentToken->second == "disableDepth") {
				pass->setDisableDepth();
			}
			else if (currentToken->second == "disableBlend") {
				pass->setDisableBlend();
			}
			else if (currentToken->second == "front") {
				std::string polyMode;
				
				stepToken();
				polyMode = getStringToken("pass.front: expecting string value for mode");
				
				pass->setFrontMode(TGen::StringToPolygonFaceMode(polyMode));
			}
			else if (currentToken->second == "back") {
				std::string polyMode;
				
				stepToken();
				polyMode = getStringToken("pass.back: expecting string value for mode");
				
				pass->setBackMode(TGen::StringToPolygonFaceMode(polyMode));
			}
			else if (currentToken->second == "depthPass") {
				stepToken();
				
				pass->setDepthFunc(TGen::StringToCompareFunc(getStringToken("pass.depthPass: expecting string value for compare func")));
			}
			else if (currentToken->second == "blendFunc") {
				stepToken();
				std::string source, dest;
				
				source = getStringToken("pass.blend: expecting string value for func");
				
				stepToken();
				if (currentToken->first == TGen::TokenValueString || currentToken->first == TGen::TokenQuote)
					dest = currentToken->second;
				
				TGen::BlendFunc fixedSource, fixedDest;
				
				if (source == "additive" || source == "add") {
					fixedSource = TGen::BlendOne;
					fixedDest = TGen::BlendOne;
				}
				else if (source == "interpolate" || source == "default" || source == "blend") {
					fixedSource = TGen::BlendSourceAlpha;
					fixedDest = TGen::BlendOneMinusSourceAlpha;
				}
				else if (source == "filter") {
					fixedSource = TGen::BlendDestColor;
					fixedDest = TGen::BlendZero;
				}
				else {
					fixedSource = TGen::StringToBlendFunc(source);
					fixedDest = TGen::StringToBlendFunc(dest);
				}
				
				pass->setBlendFunc(fixedSource, fixedDest);
			}
			else {
				throw TGen::RuntimeException("MaterialParser::ParsePassBlock", "not expecting '" + currentToken->second + "'!");							
			}
		}
		else if (currentToken->first != TGen::MaterialTokenEndOfLine) {
			throw TGen::RuntimeException("MaterialParser::ParsePassBlock", "not expecting '" + currentToken->second + "'!");			
		}
		
		tokens.stepToken(currentToken, endIter);		
	}
	
	if (currentToken->first == TGen::MaterialTokenBlockEnd)
		tokens.stepToken(currentToken, endIter);	
}

void TGen::MaterialParser::parseTexunitBlock(TGen::PassTextureUnit * unit, TGen::Material * material) {
	while (currentToken != endIter && currentToken->first != TGen::MaterialTokenBlockEnd) {
		if (currentToken->first == TGen::TokenValueString) {
			if (currentToken->second == "texCoordGen") {
				std::string genU, genV;

				stepToken();
				genU = getStringToken("texunit.texCoordGen: expecting string value for U");
				stepToken();
				genV = getStringToken("texunit.texCoordGen: expecting string value for V");
				
				unit->setTexCoordGen(genU, genV);
			}
			else if (currentToken->second == "translate" || currentToken->second == "scroll") {
				std::string offU, offV;
				float u = 0.0f, v = 0.0f;
				bool scroll = currentToken->second == "scroll";
				
				TGen::ScalarGenerator * genU = NULL, * genV = NULL;

				stepToken();
				if (currentToken->second == "wave") {
					stepToken();
					genU = parseWaveGenerator();
				}
				else {
					offU = getNumericToken("texunit.translate: expecting numeric or wave value for U offset");
				
					std::stringstream ss;
					ss << offU;
					ss >> u;
				}
				
				stepToken();
				if (currentToken->second == "wave") {
					stepToken();
					genV = parseWaveGenerator();
				}
				else {
					offV = getNumericToken("texunit.translate: expecting numeric or wave value for V offset");

					std::stringstream ss;
					ss << offV;
					ss >> v;
				}
				
				TGen::TextureTranslate * translator = new TGen::TextureTranslate(genU, genV, scroll);
				translator->setCoord(u, v);
				
				unit->addTextureTransformer(translator);
			}
			else if (currentToken->second == "scale" || currentToken->second == "stretch") {
				std::string u, v;
				float uNum = 0.0f, vNum = 0.0f;
				bool centered = currentToken->second == "stretch";
				
				TGen::ScalarGenerator * genU = NULL, * genV = NULL;
				
				stepToken();
				if (currentToken->second == "wave") {
					stepToken();
					genU = parseWaveGenerator();
				}
				else {
					u = getNumericToken("texunit.scale: expecting numeric or wave value for U");
					
					std::stringstream ss;
					ss << u;
					ss >> uNum;
				}
				
				stepToken();
				if (currentToken->first == TGen::MaterialTokenEndOfLine) {
					genV = genU;
					vNum = uNum;
				}
				else {
					if (currentToken->second == "wave") {
						stepToken();
						genV = parseWaveGenerator();
					}
					else {
						v = getNumericToken("texunit.scale: expecting numeric or wave value for V");
						
						std::stringstream ss;
						ss << v;
						ss >> vNum;
					}
				}
				
				
				TGen::TextureScale * scaler = new TGen::TextureScale(genU, genV, centered);
				scaler->setScale(uNum, vNum);
				
				unit->addTextureTransformer(scaler);
			}
			else if (currentToken->second == "rotate" || currentToken->second == "spin") {
				float u = 0.0f;
				bool centered = currentToken->second == "spin";
				
				TGen::ScalarGenerator * genRot = NULL;
				stepToken();
				if (currentToken->second == "wave") {
					stepToken();
					genRot = parseWaveGenerator();
				}
				else {
					std::string uName = getNumericToken("texunit.rotate: expecting numeric or wave value for U");
					
					std::stringstream ss;
					ss << uName;
					ss >> u;
				}
				
				TGen::TextureRotate * rotator = new TGen::TextureRotate(genRot, centered);
				rotator->setSpeed(u);
				
				unit->addTextureTransformer(rotator);				
			}
			else if (currentToken->second == "sampler") {
				std::string samplerName;
				
				stepToken();
				samplerName = getStringToken("texunit.sampler: expecting string value for sampler name");
				
				unit->setSampler(samplerName);
			}
			else if (currentToken->second == "minFilter") {
				stepToken();
				std::string filter = getStringToken("texunit.minFilter: expecting string value for minification filter");
				
				unit->setMinFilter(TGen::StringToTextureFilter(filter));
			}
			else if (currentToken->second == "magFilter") {
				stepToken();
				std::string filter = getStringToken("texunit.magFilter: expecting string value for magnification filter");
				
				unit->setMagFilter(TGen::StringToTextureFilter(filter));
			}
			
			else {
				throw TGen::RuntimeException("MaterialParser::ParseTexunitBlock", "not expecting '" + currentToken->second + "'!");
			}
		}
		else if (currentToken->first != TGen::MaterialTokenEndOfLine) {
			throw TGen::RuntimeException("MaterialParser::ParseTexunitBlock", "not expecting '" + currentToken->second + "'!");			
		}
		
		tokens.stepToken(currentToken, endIter);		
	}
	
	if (currentToken->first == TGen::MaterialTokenBlockEnd)
		tokens.stepToken(currentToken, endIter);	
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
			tokens.stepToken(currentToken, endIter);
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
			tokens.stepToken(currentToken, endIter);
		}
	}
	
	if (currentToken->first == TGen::TokenValueNumeric)
		ret = currentToken->second;
	else
		throw TGen::RuntimeException("MaterialParser::getNumericTokenk", name + ", not '" + currentToken->second + "' (") << currentToken->first << ")";
	
	return ret;
}

void TGen::MaterialParser::stepToken() {
	tokens.stepToken(currentToken, endIter);
}

TGen::WaveGenerator * TGen::MaterialParser::parseWaveGenerator() {
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
	else if (type == "inversesawtooth")
		ret = new TGen::WaveGenerator(TGen::WaveInverseSawtooth, baseNumber, amplitudeNumber, phaseNumber, frequencyNumber);				
	else if (type == "triangle")
		ret = new TGen::WaveGenerator(TGen::WaveTriangle, baseNumber, amplitudeNumber, phaseNumber, frequencyNumber);				
	else
		throw TGen::RuntimeException("MaterialParser::ParseWaveGenerator", "invalid wave type: '" + currentToken->second + "'!");
	
	return ret;
}

TGen::Color TGen::MaterialParser::parseColor() {
	scalar r = TGen::lexical_cast<scalar>(getNumericToken("parseColor: expecting numeric R value "));
	stepToken();
	scalar g = TGen::lexical_cast<scalar>(getNumericToken("parseColor: expecting numeric G value "));
	stepToken();
	scalar b = TGen::lexical_cast<scalar>(getNumericToken("parseColor: expecting numeric B value "));
	
	return TGen::Color(r, g, b);
}




void TGen::MaterialParser::addShaderPermutation(const std::string & params, int id) {
	shaderPermutations.push_back(ShaderPermutation(params, id));
}


