/*
 *  passlist.cpp
 *  TGen Graphics
 *
 *  Created by Peter Backman on 8/6/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "passlist.h"
#include "renderer.h"
#include "renderable.h"
#include "pass.h"
#include "material.h"

TGen::PassList::PassList()
	: sortLevel(TGen::MaterialSortOpaque)
{
	
}

TGen::PassList::~PassList() {
	for (int i = 0; i < passes.size(); ++i)
		delete passes[i];
}

void TGen::PassList::addPass(TGen::Pass * pass) {
	passes.push_back(pass);
}

void TGen::PassList::render(TGen::Renderer & renderer, const TGen::Renderable & renderable, TGen::Texture ** textureTypes, TGen::ShaderVariableUpdater * varupdater) {
	if (passes.empty())
		return;
	
	//	throw TGen::RuntimeException("PassList::Render", "no passes to render");
	
	TGen::Time start = TGen::Time::Now();
	renderable.preRender(renderer);
	//std::cout << "prepare: " << std::fixed << TGen::Time::Now() - start << std::endl;
	
	start = TGen::Time::Now();
	for (int i = 0; i < passes.size(); ++i) {
		renderer.setRenderContext(passes[i]->getRenderContext(), textureTypes);

		if (varupdater)
			passes[i]->updateVariables(varupdater);
				
		renderable.render(renderer);
	}

	//std::cout << "render: " << std::fixed << TGen::Time::Now() - start << std::endl;
}

void TGen::PassList::link(TGen::MaterialLinkCallback & callback) {
	PassVector::iterator iter = passes.begin();
	for (; iter != passes.end(); ++iter) {
		(*iter)->link(callback);
	}
}

void TGen::PassList::update(scalar time) {
	PassVector::iterator iter = passes.begin();
	for (; iter != passes.end(); ++iter) {
		(*iter)->update(time);
	}	
}

void TGen::PassList::setSortLevel(int level) {
	sortLevel = level;
}

int TGen::PassList::getSortLevel() const {
	return sortLevel;
}

