/*
 *  pass.cpp
 *  TGen Graphics
 *
 *  Created by Peter Backman on 7/4/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "pass.h"
#include "renderable.h"
#include "renderer.h"
#include "material.h"
#include "shaderprogram.h"
#include "shadervariable.h"
#include "generator.h"
#include "texture.h"
#include "textureunit.h"
#include "shadervarupdater.h"

#include "texturetransformer.h"
#include "passtextureunit.h"
#include "shadermode.h"
#include "materiallinkcallback.h"
#include "typesconverter.h"

#include <iostream>



TGen::Pass::Pass() 
	: colorGen(NULL)
	, alphaGen(NULL) 
{
	
}

TGen::Pass::~Pass() {
	for (int i = 0; i < shaderVariables.size(); ++i) {
		delete shaderVariables[i]->linkedVar;
		delete shaderVariables[i];
	}
	
	for (ShaderModeMap::iterator iter = shaderModes.begin(); iter != shaderModes.end(); ++iter)
		delete iter->second;
	
	delete colorGen;
	delete alphaGen;
}



const TGen::RenderContext & TGen::Pass::getRenderContext(int shaderMode) {
	ShaderModeMap::iterator iter = shaderModes.find(shaderMode);
	TGenAssert(iter != shaderModes.end());
	//TGenAssert(iter->second.shader);
	for (int i = 0; i < textureUnits.size(); ++i)
		textureUnits[i]->updateShaderVariables();
	
	renderContext.shader = iter->second->getShader();

	return renderContext;
}

void TGen::Pass::setColor(const std::string & r, const std::string & g, const std::string & b) {
	delete colorGen;
	colorGen = NULL;
	
	std::stringstream ss;
	scalar rNum, gNum, bNum;
	ss << r << " " << g << " " << b;
	ss >> rNum >> gNum >> bNum;
	
	renderContext.frontColor.r = rNum;
	renderContext.frontColor.g = gNum;
	renderContext.frontColor.b = bNum;
}

void TGen::Pass::setColorFromVertex() {
	renderContext.colorFromVertex = true;
}

void TGen::Pass::setLightDiffuse(const TGen::Color & diffuse) {
	renderContext.lightMaterial.diffuse = diffuse;
}

void TGen::Pass::setLightSpecular(const TGen::Color & specular) {
	renderContext.lightMaterial.specular = specular;
}

void TGen::Pass::setLightShininess(scalar shininess) {
	renderContext.lightMaterial.shininess = shininess;
}


void TGen::Pass::setAlpha(const std::string & a) {
	delete alphaGen;
	alphaGen = NULL;
	
	std::stringstream ss;
	scalar aNum;
	ss << a;
	ss >> aNum;
	
	renderContext.frontColor.a = aNum;
}

void TGen::Pass::setShader(const std::string & name, int mode) {
	//shaderName = name;	
	shaderModes.insert(std::make_pair(mode, new ShaderMode(name)));
}

void TGen::Pass::setColorGenerator(TGen::ColorGenerator * gen) {
	delete colorGen;	
	colorGen = gen;
}

void TGen::Pass::setAlphaGenerator(TGen::ScalarGenerator * gen) {
	delete alphaGen;
	alphaGen = gen;
}

void TGen::Pass::addTextureUnit(TGen::PassTextureUnit * textureUnit) {
	textureUnits.push_back(textureUnit);
}

int TGen::Pass::getNumTextureUnits() const {
	return textureUnits.size();
}

void TGen::Pass::link(TGen::MaterialLinkCallback & callback) {
	//std::cout << "linking shader " << shaderName << std::endl;
	
	for (ShaderModeMap::iterator iter = shaderModes.begin(); iter != shaderModes.end(); ++iter)
		iter->second->link(callback);

	
	renderContext.textureUnits.clear();
	
	TextureList::iterator iter = textureUnits.begin();
	for (; iter != textureUnits.end(); ++iter) {
		//std::cout << "linking unit " << (*iter)->unit << " with " << (*iter)->textureName << std::endl;
		TGen::TextureUnit * newUnit = NULL;
		
		int texType = 0;
		bool nullTexture = false;
		
		if ((*iter)->getTextureName() == "none" || (*iter)->getTextureName() == "NULL") {
			newUnit = new TGen::TextureUnit((*iter)->getUnit(), NULL);
			texType = 0;
			nullTexture = true;
		}
		else {
			texType = callback.getTextureType((*iter)->getTextureName());
		}
		
		if (texType > 0) {
			newUnit = new TGen::TextureUnit((*iter)->getUnit(), texType);
		}
		else if (!nullTexture) {
			newUnit = new TGen::TextureUnit((*iter)->getUnit(), callback.getTexture((*iter)->getTextureName()));
			
			if (newUnit->getTexture())
				newUnit->getTexture()->setWrapMode((*iter)->wrapU, (*iter)->wrapV);
		}
		
		
		// TODO: don't add transform shadervariables here, just link them
		for (ShaderModeMap::iterator iter2 = shaderModes.begin(); iter2 != shaderModes.end(); ++iter2) {
			try {
				std::cerr << (*iter)->getSamplerName() << std::endl;
				
				TGen::ShaderProgram * shader = iter2->second->getShader();
				
				if (shader) {
					TGen::ShaderVariable * var = shader->createVariable((*iter)->getSamplerName() + "Transform");
					(*iter)->addTransformBinder(var);
				}
			}
			catch (...) {
				std::cout << "Warning, failed to create transform binder for " << (*iter)->getSamplerName() + "Transform" << std::endl;
			}
		}
		
		newUnit->setCoordGen((*iter)->genU, (*iter)->genV);
		(*iter)->setLinkedUnit(newUnit);
		
		renderContext.addTextureUnit(newUnit);		
		
		/*if (renderContext.shader) {
			if (!(*iter)->samplerName.empty()) {
				DEBUG_PRINT("[material]: setting '" << (*iter)->samplerName << "' to " << (*iter)->unit);
				renderContext.shader->getUniform((*iter)->samplerName).setInt((*iter)->unit);
			}
		
			for (int i = 0; i < shaderVariables.size(); ++i) {
				if (!shaderVariables[i]->linkedVar) {
					DEBUG_PRINT("[material]: binding '" << shaderVariables[i]->name << "' to '" << shaderVariables[i]->linkId << "'");
					shaderVariables[i]->linkedVar = renderContext.shader->createVariable(shaderVariables[i]->name);
				}				
			}
		}*/
		
		for (ShaderModeMap::iterator iter2 = shaderModes.begin(); iter2 != shaderModes.end(); ++iter2) {
			TGen::ShaderProgram * shader = iter2->second->getShader();
			
			if (shader) {
			/*	if (!(*iter)->samplerName.empty()) {
					//DEBUG_PRINT("[material]: setting '" << (*iter)->samplerName << "' to " << (*iter)->unit);
					shader->getUniform((*iter)->samplerName).setInt((*iter)->unit);
				}*/
			
				
				for (int i = 0; i < shaderVariables.size(); ++i) {
					if (!shaderVariables[i]->linkedVar) {
						//DEBUG_PRINT("[material]: binding '" << shaderVariables[i]->name << "' to '" << shaderVariables[i]->linkId << "'");
						shaderVariables[i]->linkedVar = shader->createVariable(shaderVariables[i]->name);
					}				
				}
			}
			
			// En "map" skapar en shadervarupdater för varje shadermode som sätter korrekt sampler. (en vanlig set int)
			// texunit behöver också göra det....
		
			// TODO: shaderupdaters, one for every shader variable. ie, one for every var and shader mode.
		}
	}
}

void TGen::Pass::updateVariables(TGen::ShaderVariableUpdater * varupdater) {
	if (varupdater) {
		for (int i = 0; i < shaderVariables.size(); ++i) {
			varupdater->updateShaderVariable(*shaderVariables[i]->linkedVar, shaderVariables[i]->linkId);
		}
	}
	
	// update shader variables::: 
	

	TextureList::iterator iter = textureUnits.begin();
	for (; iter != textureUnits.end(); ++iter) {
		//(*iter)->update();	
		
		for (ShaderModeMap::iterator iter2 = shaderModes.begin(); iter2 != shaderModes.end(); ++iter2) {
			iter2->second->update();
			/*TGen::ShaderProgram * shader = iter2->second.shader;
			
			if (shader) {
				if (!(*iter)->samplerName.empty()) {
					//DEBUG_PRINT("[material]: setting '" << (*iter)->samplerName << "' to " << (*iter)->unit);
					shader->getUniform((*iter)->samplerName).setInt((*iter)->unit);
				}
			}*/
			
			
		}
	}
}


void TGen::Pass::setDepthFunc(const std::string & func) {
	if (func == "never")
		renderContext.depthFunc = TGen::CompareNever;
	else if (func == "less")
		renderContext.depthFunc = TGen::CompareLess;
	else if (func == "equal")
		renderContext.depthFunc = TGen::CompareEqual;
	else if (func == "lequal")
		renderContext.depthFunc = TGen::CompareLessOrEqual;
	else if (func == "greater")
		renderContext.depthFunc = TGen::CompareGreater;
	else if (func == "nequal")
		renderContext.depthFunc = TGen::CompareNotEqual;
	else if (func == "gequal")
		renderContext.depthFunc = TGen::CompareGreaterOrEqual;
	else if (func == "always")
		renderContext.depthFunc = TGen::CompareAlways;
	else
		throw TGen::RuntimeException("Pass::setDepthFunc", "invalid compare function: '" + func + "'!");
}

void TGen::Pass::setFrontMode(const std::string & mode) {
	if (mode == "cull")
		renderContext.front = TGen::PolygonFaceCull;
	else if (mode == "lines" || mode == "line")
		renderContext.front = TGen::PolygonFaceLines;
	else if (mode == "point" || mode == "points")
		renderContext.front = TGen::PolygonFacePoints;
	else if (mode == "fill")
		renderContext.front = TGen::PolygonFaceFill;
	else
		throw TGen::RuntimeException("Pass::setFrontMode", "invalid front mode: '" + mode + "'!");
}

void TGen::Pass::setBackMode(const std::string & mode) {
	if (mode == "cull")
		renderContext.back = TGen::PolygonFaceCull;
	else if (mode == "lines" || mode == "line")
		renderContext.back = TGen::PolygonFaceLines;
	else if (mode == "point" || mode == "points")
		renderContext.back = TGen::PolygonFacePoints;
	else if (mode == "fill")
		renderContext.back = TGen::PolygonFaceFill;
	else
		throw TGen::RuntimeException("Pass::setBackMode", "invalid back mode: '" + mode + "'!");	
}

void TGen::Pass::setNoDepthWrite() {
	renderContext.depthWrite = false;
}

void TGen::Pass::setDisableDepth() {
	renderContext.disableDepth = true;
}

void TGen::Pass::setDisableBlend() {
	renderContext.disableBlend = true;
}

void TGen::Pass::setBlendFunc(const std::string & source, const std::string & dest) {
	if (source == "additive" || source == "add") {
		renderContext.blendSrc = TGen::BlendOne;
		renderContext.blendDst = TGen::BlendOne;
	}
	else if (source == "interpolate" || source == "default" || source == "blend") {
		renderContext.blendSrc = TGen::BlendSourceAlpha;
		renderContext.blendDst = TGen::BlendOneMinusSourceAlpha;
	}
	else if (source == "filter") {
		renderContext.blendSrc = TGen::BlendDestColor;
		renderContext.blendDst = TGen::BlendZero;
	}
	else {
		renderContext.blendSrc = StringToBlendFunc(source);
		renderContext.blendDst = StringToBlendFunc(dest);		
	}
}

void TGen::Pass::update(scalar time) {
	if (colorGen) {
		TGen::Color newColor = colorGen->getColor(time, 1.0);
		renderContext.frontColor.r = newColor.r;
		renderContext.frontColor.g = newColor.g;
		renderContext.frontColor.b = newColor.b;
	}
	
	if (alphaGen) {
		renderContext.frontColor.a = alphaGen->getValue(time);
	}
	
	
	// TODO: två updated, en som uppdaterar transformeringen och en som uppdaterar shadervars
	//        sen städa upp i alla klasserna som har publika members! och vissa klasser måste ut i flera filer
	for (int i = 0; i < textureUnits.size(); ++i) {
		textureUnits[i]->update(time);
	}
}


void TGen::Pass::addShaderVariable(const std::string & varname, const std::string & linkId) {
	TGen::PassShaderVariable * passVar = new TGen::PassShaderVariable;
	passVar->name = varname;
	passVar->linkId = linkId;
	
	shaderVariables.push_back(passVar);
}


void TGen::Pass::addShaderUpdater(TGen::ShaderUpdater * updater) {
	for (ShaderModeMap::iterator iter = shaderModes.begin(); iter != shaderModes.end(); ++iter) {
		iter->second->addShaderUpdater(updater->clone());
	}
	
	delete updater;
}


