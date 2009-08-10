/*
 *  passtextureunit.cpp
 *  TGen Graphics
 *
 *  Created by Peter Backman on 1/7/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include "passtextureunit.h"
#include "textureunit.h"
#include "texturetransformer.h"
#include "shadervariable.h"

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
	else if (fixedU == "repeat")
		this->wrapU = TGen::TextureWrapRepeat;
	
	if (fixedV == "clamp")
		this->wrapV = TGen::TextureWrapClamp;
	else if (fixedV == "repeat")
		this->wrapV = TGen::TextureWrapRepeat;
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

void TGen::PassTextureUnit::addTextureTransformer(TGen::TextureTransformer * transformer) {
	transformers.push_back(transformer);
}

void TGen::PassTextureUnit::update(scalar time) {
	if (!texunit)
		return;
	
	texunit->transform = TGen::Matrix4x4::Identity;
	texunit->transformed = false;
	
	bool lastCentered = false;
	
	for (int i = 0; i < transformers.size(); ++i) {
		if (transformers[i]->shouldCenter() && !lastCentered)
			texunit->transform *= TGen::Matrix4x4::Translation(TGen::Vector2(0.5f, 0.5f));
		else if (!transformers[i]->shouldCenter() && lastCentered)
			texunit->transform *= TGen::Matrix4x4::Translation(TGen::Vector2(-0.5f, -0.5f));
		
		lastCentered = transformers[i]->shouldCenter();
		
		transformers[i]->applyTransform(texunit->transform, time);
		texunit->transformed = true;
	}
	
	if (lastCentered)
		texunit->transform *= TGen::Matrix4x4::Translation(TGen::Vector2(-0.5f, -0.5f));	
}

void TGen::PassTextureUnit::updateShaderVariables() {
	if (!texunit) {
		for (int i = 0; i < binders.size(); ++i)
			*binders[i] = TGen::Matrix4x4::Identity;
	}
	else {
		for (int i = 0; i < binders.size(); ++i)
			*binders[i] = texunit->transform;
	}
}

void TGen::PassTextureUnit::addTransformBinder(TGen::ShaderVariable * var) {
	binders.push_back(var);
}

void TGen::PassTextureUnit::setTextureName(const std::string & name) {
	textureName = name;
}

const std::string & TGen::PassTextureUnit::getTextureName() const {
	return textureName;
}

const std::string & TGen::PassTextureUnit::getSamplerName() const {
	return samplerName;
}

void TGen::PassTextureUnit::setMinFilter(TGen::TextureFilter minFilter) {
	this->minFilter = minFilter;
}

void TGen::PassTextureUnit::setMagFilter(TGen::TextureFilter magFilter) {
	this->magFilter = magFilter;
}

void TGen::PassTextureUnit::setLinkedUnit(TGen::TextureUnit * texunit) {
	this->texunit = texunit;
}

int TGen::PassTextureUnit::getUnit() const {
	return unit;
}

