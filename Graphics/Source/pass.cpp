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
	
	delete colorGen;
	delete alphaGen;
}



const TGen::RenderContext & TGen::Pass::getRenderContext(int shaderMode) {
	ShaderModeMap::iterator iter = shaderModes.find(shaderMode);
	TGenAssert(iter != shaderModes.end());
	//TGenAssert(iter->second.shader);
	for (int i = 0; i < textureUnits.size(); ++i) {
		textureUnits[i]->update(0.0f, false);
	}
	
	renderContext.shader = iter->second.shader;
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
	shaderModes.insert(std::make_pair(mode, ShaderMode(name)));
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
		iter->second.link(callback);

	
	renderContext.textureUnits.clear();
	
	TextureList::iterator iter = textureUnits.begin();
	for (; iter != textureUnits.end(); ++iter) {
		//std::cout << "linking unit " << (*iter)->unit << " with " << (*iter)->textureName << std::endl;
		TGen::TextureUnit * newUnit = NULL;
		
		int texType = 0;
		bool nullTexture = false;
		
		if ((*iter)->textureName == "none" || (*iter)->textureName == "NULL") {
			newUnit = new TGen::TextureUnit((*iter)->unit, NULL);
			texType = 0;
			nullTexture = true;
		}
		else {
			texType = callback.getTextureType((*iter)->textureName);
		}
		
		if (texType > 0) {
			newUnit = new TGen::TextureUnit((*iter)->unit, texType);
		}
		else if (!nullTexture) {
			newUnit = new TGen::TextureUnit((*iter)->unit, callback.getTexture((*iter)->textureName));
			
			if (newUnit->texture)
				newUnit->texture->setWrapMode((*iter)->wrapU, (*iter)->wrapV);
		}
		
		
		// TODO: don't add transform shadervariables here, just link them
		for (ShaderModeMap::iterator iter2 = shaderModes.begin(); iter2 != shaderModes.end(); ++iter2) {
			try {
				std::cerr << (*iter)->samplerName << std::endl;
				
				TGen::ShaderProgram * shader = iter2->second.shader;
				
				if (shader) {
					TGen::ShaderVariable * var = shader->createVariable((*iter)->samplerName + "Transform");
					(*iter)->binders.push_back(var);
				}
			}
			catch (...) {
				std::cout << "Warning, failed to create transform binder for " << (*iter)->samplerName + "Transform" << std::endl;
			}
		}
		
		newUnit->genU = (*iter)->genU;
		newUnit->genV = (*iter)->genV;
		(*iter)->texunit = newUnit;
		
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
			TGen::ShaderProgram * shader = iter2->second.shader;
			
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
			iter2->second.update();
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

TGen::PassTextureUnit::PassTextureUnit(int unit, const std::string & name) 
	: unit(unit)
	, textureName(name)
	, genU(TGen::TextureCoordGenBase)
	, genV(TGen::TextureCoordGenBase)
	, texunit(NULL)
	, textureType(0) 
	, wrapU(TGen::TextureWrapRepeat)
	, wrapV(TGen::TextureWrapRepeat)
{
}

TGen::PassTextureUnit::~PassTextureUnit() {
	for (int i = 0; i < transformers.size(); ++i)
		delete transformers[i];
}

void TGen::PassTextureUnit::setSampler(const std::string & sampler) {
	samplerName = sampler;
}

void TGen::PassTextureUnit::setWrap(const std::string & wrapU, const std::string & wrapV) {
	std::string fixedU, fixedV;
	fixedU = TGen::toLower(wrapU);
	fixedV = TGen::toLower(wrapV);
	
	if (fixedU == "clamp")
		this->wrapU = TGen::TextureWrapClamp;
	
	if (fixedV == "clamp")
		this->wrapV = TGen::TextureWrapClamp;
}

void TGen::PassTextureUnit::setTexCoordGen(const std::string & genU, const std::string & genV) {
	if (genU == "base")
		this->genU = TGen::TextureCoordGenBase;
	else if (genU == "object")
		this->genU = TGen::TextureCoordGenObjectLinear;
	else if (genU == "eye")
		this->genU = TGen::TextureCoordGenEyeLinear;
	else if (genU == "sphere" || genU == "environment")
		this->genU = TGen::TextureCoordGenSphereMap;
	else
		throw TGen::RuntimeException("PassTextureUnit::setTexCoordGen", "invalid value for u: '" + genU + "'");

	if (genV == "base")
		this->genV = TGen::TextureCoordGenBase;
	else if (genV == "object")
		this->genV = TGen::TextureCoordGenObjectLinear;
	else if (genV == "eye")
		this->genV = TGen::TextureCoordGenEyeLinear;
	else if (genV == "sphere" || genV == "environment")
		this->genV = TGen::TextureCoordGenSphereMap;
	else
		throw TGen::RuntimeException("PassTextureUnit::setTexCoordGen", "invalid value for v: '" + genV + "'");
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

TGen::BlendFunc TGen::Pass::StringToBlendFunc(const std::string & blend) {
	std::string fixedBlend = TGen::toLower(blend);
	
	if (fixedBlend == "zero" || fixedBlend == "0" || fixedBlend == "gl_zero")
		return TGen::BlendZero;
	else if (fixedBlend == "one" || fixedBlend == "1" || fixedBlend == "gl_one")
		return TGen::BlendOne;
	else if (fixedBlend == "destcolor" || fixedBlend == "dstcolor" || fixedBlend == "gl_dst_color")
		return TGen::BlendDestColor;
	else if (fixedBlend == "oneminusdestcolor" || fixedBlend == "1-dstcolor" || fixedBlend == "gl_one_minus_dst_color")
		return TGen::BlendOneMinusDestColor;
	else if (fixedBlend == "srcalpha" || fixedBlend == "srcalpha" || fixedBlend == "gl_src_alpha")
		return TGen::BlendSourceAlpha;
	else if (fixedBlend == "oneminussrcalpha" || fixedBlend == "1-srcalpha" || fixedBlend == "gl_one_minus_src_alpha")
		return TGen::BlendOneMinusSourceAlpha;
	else if (fixedBlend == "destalpha" || fixedBlend == "dstalpha" || fixedBlend == "gl_dst_alpha")
		return TGen::BlendDestAlpha;
	else if (fixedBlend == "oneminusdestalpha" || fixedBlend == "1-dstalpha" || fixedBlend == "gl_one_minus_dst_alpha")
		return TGen::BlendOneMinusDestAlpha;
	else if (fixedBlend == "srccolor" || fixedBlend == "srccolor" || fixedBlend == "gl_src_color")
		return TGen::BlendSourceColor;
	else if (fixedBlend == "oneminussrccolor" || fixedBlend == "1-srccolor" || fixedBlend == "gl_one_minus_src_color")
		return TGen::BlendOneMinusSourceColor;
	
	throw TGen::RuntimeException("Pass::StringToBlendFunc", "invalid blend func: '" + blend + "'!");		
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
	for (int i = 0; i < textureUnits.size(); ++i) {
		textureUnits[i]->update(time);
	}
}

void TGen::PassTextureUnit::addTexCoordTransformer(TGen::TextureCoordTransformer * transformer) {
	transformers.push_back(transformer);
}

void TGen::PassTextureUnit::update(scalar time, bool update) {
	if (!texunit) {
		for (int i = 0; i < binders.size(); ++i) {
			*binders[i] = TGen::Matrix4x4::Identity;
		}
		
		return;
	}
	
	// only gets updated with 0.0f......
	
	if (update) {
	texunit->transform = TGen::Matrix4x4::Identity;
	texunit->transformed = false;
	
	bool lastCentered = false;
	
	for (int i = 0; i < transformers.size(); ++i) {
		if (transformers[i]->centered && !lastCentered)
			texunit->transform *= TGen::Matrix4x4::Translation(TGen::Vector2(0.5f, 0.5f));
		else if (!transformers[i]->centered && lastCentered)
			texunit->transform *= TGen::Matrix4x4::Translation(TGen::Vector2(-0.5f, -0.5f));

		lastCentered = transformers[i]->centered;
		
		transformers[i]->applyTransform(texunit->transform, time);
		texunit->transformed = true;
	}
	
	if (lastCentered)
		texunit->transform *= TGen::Matrix4x4::Translation(TGen::Vector2(-0.5f, -0.5f));	
	}
	
	for (int i = 0; i < binders.size(); ++i) {
		*binders[i] = texunit->transform;
	}
}

void TGen::PassTextureUnit::setTextureName(const std::string & name) {
	textureName = name;
}

TGen::TextureCoordTransformer::TextureCoordTransformer(TGen::ScalarGenerator * genU, TGen::ScalarGenerator * genV, bool centered) 
	: genU(genU)
	, genV(genV)
	, startedAt(0.0)
	, centered(centered)
{
}

TGen::TextureCoordTransformer::~TextureCoordTransformer() {
	delete genU;
	if (genV != genU)
		delete genV;
}


TGen::TextureCoordTranslate::TextureCoordTranslate(float u, float v, bool scroll)
	: TGen::TextureCoordTransformer(NULL, NULL, false)
	, u(u)
	, v(v)
	, scroll(scroll) 
{
}

TGen::TextureCoordTranslate::TextureCoordTranslate(TGen::ScalarGenerator * genU, TGen::ScalarGenerator * genV, bool scroll)
	: TGen::TextureCoordTransformer(genU, genV, false)
	, scroll(scroll) 
{
}

void TGen::TextureCoordTranslate::applyTransform(TGen::Matrix4x4 & matrix, scalar time) {
	if (!scroll) {
		float fixedU = 0.0f, fixedV = 0.0f;
		
		if (!genU)
			fixedU = u;
		else
			fixedU = genU->getValue(time);
		
		if (!genV)
			fixedV = v;
		else
			fixedV = genV->getValue(time);
				
		//fixedU = fixedU - floor(fixedU);		// fix range. crappy code
		//fixedV = fixedV - floor(fixedV);		
		
		matrix *= TGen::Matrix4x4::Translation(TGen::Vector2(-fixedU, fixedV));		
	}
	else {
		
		float fixedU = 0.0f, fixedV = 0.0f;
		
		if (!genU)
			fixedU = (time - startedAt) * u;
		else
			fixedU = genU->getValue(time);
		
		if (!genV)
			fixedV = (time - startedAt) * v;
		else
			fixedV = genV->getValue(time);				

		
		//	fixedU = fixedU - floor(fixedU);		// fix range
		//		fixedV = fixedV - floor(fixedV);
		
		matrix *= TGen::Matrix4x4::Translation(TGen::Vector2(-fixedU, fixedV));
	}
}


TGen::TextureCoordScale::TextureCoordScale(float u, float v, bool centered)
	: TGen::TextureCoordTransformer(NULL, NULL, centered)
	, u(u)
	, v(v)
{
}

TGen::TextureCoordScale::TextureCoordScale(TGen::ScalarGenerator * genU, TGen::ScalarGenerator * genV, bool centered)
	: TGen::TextureCoordTransformer(genU, genV, centered)
	, u(0.0f)
	, v(0.0f)
{
}

void TGen::TextureCoordScale::applyTransform(TGen::Matrix4x4 & matrix, scalar time) {
	float fixedU = 0.0f, fixedV = 0.0f;
	
	if (!genU)
		fixedU = u;
	else
		fixedU = genU->getValue(time);
	
	if (!genV)
		fixedV = v;
	else
		fixedV = genV->getValue(time);
	
	if (fixedU < 0.0f)
		fixedU = 0.0f;
	if (fixedV < 0.0f)
		fixedV = 0.0f;
	
	//if (centered) matrix *= TGen::Matrix4x4::Translation(TGen::Vector2(0.5f, 0.5f));
	matrix *= TGen::Matrix4x4::Scaling(TGen::Vector3(fixedU, fixedV, 1.0));
	//if (centered) matrix *= TGen::Matrix4x4::Translation(TGen::Vector2(-0.5f, -0.5f));
}

TGen::TextureCoordRotate::TextureCoordRotate(float speed, bool centered)
	: TGen::TextureCoordTransformer(NULL, NULL, centered)
	, speed(speed)
{
}

TGen::TextureCoordRotate::TextureCoordRotate(TGen::ScalarGenerator * genRot, bool centered) 
	: TGen::TextureCoordTransformer(genRot, NULL, centered)
	, speed(0.0f)
{
}

void TGen::TextureCoordRotate::applyTransform(TGen::Matrix4x4 & matrix, scalar time) {
	//if (centered) matrix *= TGen::Matrix4x4::Translation(TGen::Vector2(0.5f, 0.5f));

	if (!genU) {
		float degrees = (time - startedAt) * speed;
		
		degrees = degrees - floor(degrees / 360.0f) * 360.0f;
		//if (speed < 0.0)
	//		degrees = -degrees;
		
		matrix *= TGen::Matrix4x4::Rotation(TGen::Vector3(0.0f, 0.0f, 1.0f), TGen::Degree(-degrees));
	}
	else {
		matrix *= TGen::Matrix4x4::Rotation(TGen::Vector3(0.0f, 0.0f, 1.0f), TGen::Degree(genU->getValue(time)));
	}

	//if (centered) matrix *= TGen::Matrix4x4::Translation(TGen::Vector2(-0.5f, -0.5f));
}

void TGen::Pass::addShaderVariable(const std::string & varname, const std::string & linkId) {
	TGen::PassShaderVariable * passVar = new TGen::PassShaderVariable;
	passVar->name = varname;
	passVar->linkId = linkId;
	
	shaderVariables.push_back(passVar);
}

void TGen::ShaderMode::link(TGen::MaterialLinkCallback & callback) {
	if (name == "fixed")
		shader = NULL;
	else
		shader = callback.getShaderProgram(name);
	
	for (ShaderUpdaterList::iterator iter = shaderUpdaters.begin(); iter != shaderUpdaters.end();) {
		try {
			(*iter)->link(shader);
			++iter;
		}
		catch (const TGen::RuntimeException & error) {
			std::cout << "Error while linking shader updaters: \"" + std::string(error.what()) + "\", removed variable" << std::endl;
			
			delete *iter;
			iter = shaderUpdaters.erase(iter);
		}
	}
}

void TGen::ShaderMode::update() {
	for (ShaderUpdaterList::iterator iter = shaderUpdaters.begin(); iter != shaderUpdaters.end(); ++iter)
		(*iter)->update();	
}

void TGen::ShaderMode::addShaderUpdater(TGen::ShaderUpdater * updater) {
	shaderUpdaters.push_back(updater);
}

void TGen::Pass::addShaderUpdater(TGen::ShaderUpdater * updater) {
	for (ShaderModeMap::iterator iter = shaderModes.begin(); iter != shaderModes.end(); ++iter) {
		iter->second.addShaderUpdater(updater->clone());
	}
	
	delete updater;
}


