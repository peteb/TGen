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
#include <iostream>

TGen::PassList::PassList() {
	
}

TGen::PassList::~PassList() {
	for (int i = 0; i < passes.size(); ++i)
		delete passes[i];
}

TGen::Pass::Pass() {
	
}

TGen::Pass::~Pass() {
	
}

void TGen::PassList::addPass(TGen::Pass * pass) {
	passes.push_back(pass);
}

void TGen::PassList::Render(TGen::Renderer & renderer, TGen::Renderable & renderable) {
	if (passes.empty())
		throw TGen::RuntimeException("PassList::Render", "no passes to render");
	
	renderable.PrepareRender(renderer);
	
	for (int i = 0; i < passes.size(); ++i) {
		renderer.setRenderContext(passes[i]->getRenderContext());
		renderable.Render(renderer);
	}
}

const TGen::RenderContext & TGen::Pass::getRenderContext() const {
	return renderContext;
}

void TGen::Pass::setColor(const std::string & r, const std::string & g, const std::string & b) {
	std::cout << "R: " << r << " G: " << g << " B: " << b << std::endl;
}

void TGen::Pass::setShader(const std::string & name) {
	shaderName = name;
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

void TGen::Pass::Link(TGen::MaterialLinkCallback & callback) {
	std::cout << "linking shader " << shaderName << std::endl;
	if (shaderName == "fixed")
		renderContext.shader = NULL;
	else
		renderContext.shader = callback.getShaderProgram(shaderName);
	
	renderContext.textureUnits.clear();
	
	TextureList::iterator iter = textureUnits.begin();
	for (; iter != textureUnits.end(); ++iter) {
		//std::cout << "linking unit " << (*iter)->unit << " with " << (*iter)->textureName << std::endl;
		TGen::TextureUnit * newUnit = NULL;
		
		if ((*iter)->textureName == "none")
			newUnit = new TGen::TextureUnit((*iter)->unit, NULL);			
		else
			newUnit = new TGen::TextureUnit((*iter)->unit, callback.getTexture((*iter)->textureName));
		
		newUnit->genU = (*iter)->genU;
		newUnit->genV = (*iter)->genV;
		
		renderContext.AddTextureUnit(newUnit);		
		
		if (renderContext.shader && !(*iter)->samplerName.empty()) {
			std::cout << "setting '" << (*iter)->samplerName << "' to " << (*iter)->unit << std::endl;
			renderContext.shader->getUniform((*iter)->samplerName).setInt((*iter)->unit);
		}
	}
	
	
}

TGen::PassTextureUnit::PassTextureUnit(int unit, const std::string & name) : unit(unit), textureName(name), genU(TGen::TextureCoordGenBase), genV(TGen::TextureCoordGenBase) {}

void TGen::PassTextureUnit::setSampler(const std::string & sampler) {
	samplerName = sampler;
}


void TGen::PassTextureUnit::setTexCoordGen(const std::string & genU, const std::string & genV) {
	if (genU == "base")
		this->genU = TGen::TextureCoordGenBase;
	else if (genU == "object")
		this->genU = TGen::TextureCoordGenObjectLinear;
	else if (genU == "eye")
		this->genU = TGen::TextureCoordGenEyeLinear;
	else if (genU == "sphere")
		this->genU = TGen::TextureCoordGenSphereMap;
	else
		throw TGen::RuntimeException("PassTextureUnit::setTexCoordGen", "invalid value for u: '" + genU + "'");

	if (genV == "base")
		this->genV = TGen::TextureCoordGenBase;
	else if (genV == "object")
		this->genV = TGen::TextureCoordGenObjectLinear;
	else if (genV == "eye")
		this->genV = TGen::TextureCoordGenEyeLinear;
	else if (genV == "sphere")
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

