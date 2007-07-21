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
#include <iostream>

TGen::PassList::PassList() {
	
}

TGen::PassList::~PassList() {
	for (int i = 0; i < passes.size(); ++i)
		delete passes[i];
}

TGen::Pass::Pass() : colorGen(NULL), alphaGen(NULL) {
	
}

TGen::Pass::~Pass() {
	delete colorGen;
	delete alphaGen;
}

void TGen::PassList::addPass(TGen::Pass * pass) {
	passes.push_back(pass);
}

void TGen::PassList::Render(TGen::Renderer & renderer, const TGen::Renderable & renderable, TGen::Texture ** textureTypes) {
	if (passes.empty())
		throw TGen::RuntimeException("PassList::Render", "no passes to render");
	
	renderable.PrepareRender(renderer);
	
	for (int i = 0; i < passes.size(); ++i) {
		renderer.setRenderContext(passes[i]->getRenderContext(), textureTypes);
		renderable.Render(renderer);
	}
}

const TGen::RenderContext & TGen::Pass::getRenderContext() const {
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

void TGen::Pass::setAlpha(const std::string & a) {
	delete alphaGen;
	alphaGen = NULL;
	
	std::stringstream ss;
	scalar aNum;
	ss << a;
	ss >> aNum;
	
	renderContext.frontColor.a = aNum;
}

void TGen::Pass::setShader(const std::string & name) {
	shaderName = name;
}

void TGen::Pass::setColorGenerator(TGen::ColorGenerator * gen) {
	delete colorGen;	
	colorGen = gen;
}

void TGen::Pass::setAlphaGenerator(TGen::ScalarGenerator * gen) {
	delete alphaGen;
	alphaGen = gen;
}

void TGen::Pass::AddTextureUnit(TGen::PassTextureUnit * textureUnit) {
	textureUnits.push_back(textureUnit);
}


void TGen::PassList::Link(TGen::MaterialLinkCallback & callback) {
	PassVector::iterator iter = passes.begin();
	for (; iter != passes.end(); ++iter) {
		(*iter)->Link(callback);
	}
}

void TGen::PassList::Update(scalar time) {
	PassVector::iterator iter = passes.begin();
	for (; iter != passes.end(); ++iter) {
		(*iter)->Update(time);
	}	
}

void TGen::Pass::Link(TGen::MaterialLinkCallback & callback) {
	//std::cout << "linking shader " << shaderName << std::endl;
	if (shaderName == "fixed")
		renderContext.shader = NULL;
	else
		renderContext.shader = callback.getShaderProgram(shaderName);
	
	renderContext.textureUnits.clear();
	
	TextureList::iterator iter = textureUnits.begin();
	for (; iter != textureUnits.end(); ++iter) {
		//std::cout << "linking unit " << (*iter)->unit << " with " << (*iter)->textureName << std::endl;
		TGen::TextureUnit * newUnit = NULL;
		
		int texType = callback.getTextureType((*iter)->textureName);
		
		if ((*iter)->textureName == "none" || texType != 0) {
			newUnit = new TGen::TextureUnit((*iter)->unit, NULL);
		}
		else {
			newUnit = new TGen::TextureUnit((*iter)->unit, callback.getTexture((*iter)->textureName));
			
			if (newUnit->texture)
				newUnit->texture->setWrap((*iter)->wrapU, (*iter)->wrapV);
		}
		
		
		newUnit->genU = (*iter)->genU;
		newUnit->genV = (*iter)->genV;
		(*iter)->texunit = newUnit;
		
		renderContext.AddTextureUnit(newUnit);		
		
		if (renderContext.shader && !(*iter)->samplerName.empty()) {
			std::cout << "setting '" << (*iter)->samplerName << "' to " << (*iter)->unit << std::endl;
			renderContext.shader->getUniform((*iter)->samplerName).setInt((*iter)->unit);
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
	
	std::cout << "setting ggggggg to " << wrapU << " ... " << fixedV << std::endl;
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

void TGen::Pass::Update(scalar time) {
	if (colorGen) {
		TGen::Color newColor = colorGen->getColor(time, 1.0);
		renderContext.frontColor.r = newColor.r;
		renderContext.frontColor.g = newColor.g;
		renderContext.frontColor.b = newColor.b;
	}
	
	if (alphaGen) {
		renderContext.frontColor.a = alphaGen->getValue(time);
	}
	
	for (int i = 0; i < textureUnits.size(); ++i) {
		textureUnits[i]->Update(time);
	}
}

void TGen::PassTextureUnit::AddTexCoordTransformer(TGen::TextureCoordTransformer * transformer) {
	transformers.push_back(transformer);
}

void TGen::PassTextureUnit::Update(scalar time) {
	if (!texunit)
		return;
	
	texunit->transform = TGen::Matrix4x4::Identity;
	texunit->transformed = false;
	
	for (int i = 0; i < transformers.size(); ++i) {
		transformers[i]->ApplyTransform(texunit->transform, time);
		texunit->transformed = true;
	}
}

void TGen::PassTextureUnit::setTextureName(const std::string & name) {
	textureName = name;
}

TGen::TextureCoordTransformer::TextureCoordTransformer(TGen::ScalarGenerator * genU, TGen::ScalarGenerator * genV) 
	: genU(genU), genV(genV), startedAt(0.0) {}

TGen::TextureCoordTransformer::~TextureCoordTransformer() {
	delete genU;
	if (genV != genU)
		delete genV;
}


TGen::TextureCoordTranslate::TextureCoordTranslate(float u, float v, bool scroll)
	: TGen::TextureCoordTransformer(NULL, NULL), u(u), v(v), scroll(scroll) {}

TGen::TextureCoordTranslate::TextureCoordTranslate(TGen::ScalarGenerator * genU, TGen::ScalarGenerator * genV, bool scroll)
	: TGen::TextureCoordTransformer(genU, genV), scroll(scroll) {}

void TGen::TextureCoordTranslate::ApplyTransform(TGen::Matrix4x4 & matrix, scalar time) {
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
				
		//fixedU = fixedU - floor(fixedU);		// fix range
		//fixedV = fixedV - floor(fixedV);		
		
		matrix *= TGen::Matrix4x4::Translation(TGen::Vector2(fixedU, fixedV));		
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
		
		matrix *= TGen::Matrix4x4::Translation(TGen::Vector2(fixedU, fixedV));
	}
}


TGen::TextureCoordScale::TextureCoordScale(float u, float v, bool centered)
	: TGen::TextureCoordTransformer(NULL, NULL), u(u), v(v), centered(centered)
{}

TGen::TextureCoordScale::TextureCoordScale(TGen::ScalarGenerator * genU, TGen::ScalarGenerator * genV, bool centered)
	: TGen::TextureCoordTransformer(genU, genV), u(0.0f), v(0.0f), centered(centered)
{}


void TGen::TextureCoordScale::ApplyTransform(TGen::Matrix4x4 & matrix, scalar time) {
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
	
	if (centered) matrix *= TGen::Matrix4x4::Translation(TGen::Vector2(0.5f, 0.5f));
	matrix *= TGen::Matrix4x4::Scaling(TGen::Vector2(fixedU, fixedV));
	if (centered) matrix *= TGen::Matrix4x4::Translation(TGen::Vector2(-0.5f, -0.5f));
}

TGen::TextureCoordRotate::TextureCoordRotate(float speed, bool centered)
	: TGen::TextureCoordTransformer(NULL, NULL), speed(speed), centered(centered)
{}

TGen::TextureCoordRotate::TextureCoordRotate(TGen::ScalarGenerator * genRot, bool centered) 
	: TGen::TextureCoordTransformer(genRot, NULL), speed(0.0f), centered(centered)
{}

void TGen::TextureCoordRotate::ApplyTransform(TGen::Matrix4x4 & matrix, scalar time) {
	if (centered) matrix *= TGen::Matrix4x4::Translation(TGen::Vector2(0.5f, 0.5f));

	if (!genU) {
		float degrees = (time - startedAt) * speed;
		
		degrees = degrees - floor(degrees / 360.0f) * 360.0f;
		if (speed < 0.0)
			degrees = -degrees;
		
		matrix *= TGen::Matrix4x4::Rotation(TGen::Vector3(0.0f, 0.0f, 1.0f), TGen::Degree(degrees));
	}
	else {
		matrix *= TGen::Matrix4x4::Rotation(TGen::Vector3(0.0f, 0.0f, 1.0f), TGen::Degree(genU->getValue(time)));
	}

	if (centered) matrix *= TGen::Matrix4x4::Translation(TGen::Vector2(-0.5f, -0.5f));
}



