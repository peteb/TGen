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
		renderContext.shader = callback.getShader(shaderName);
	
	renderContext.textureUnits.clear();
	
	TextureList::iterator iter = textureUnits.begin();
	for (; iter != textureUnits.end(); ++iter) {
		std::cout << "linking unit " << (*iter)->unit << " with " << (*iter)->textureName << std::endl;
		renderContext.setTextureUnit((*iter)->unit, callback.getTexture((*iter)->textureName));
	}
	
	
}

TGen::PassTextureUnit::PassTextureUnit(int unit, const std::string & name) : unit(unit), textureName(name) {}
