/*
 *  material.cpp
 *  TGen Graphics
 *
 *  Created by Peter Backman on 7/4/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "material.h"
#include "renderable.h"
#include <iostream>
#include "technique.h"
#include "pass.h"
#include "renderer.h"

TGen::Material::SpecializationMap TGen::Material::specializations;

TGen::Material::Material(const std::string & name) : minimumTechnique(2), name(name) {
	TGen::TechniqueList * newTechniques = new TGen::TechniqueList;
	TGen::Technique * newTechnique = new TGen::Technique;
	TGen::PassList * passes = new TGen::PassList;
	TGen::Pass * pass = new TGen::Pass;
	
	passes->addPass(pass);
	newTechnique->setPassList(passes, 0);
	newTechniques->setTechnique(newTechnique, 0);
	
	
	techniques[getSpecializationID("default")] = newTechniques;
}

TGen::Material::~Material() {
	
}

std::string TGen::Material::getName() const {
	return name;
}

void TGen::Material::Link(TGen::MaterialLinkCallback & callback) {
	TechniqueListMap::iterator iter = techniques.begin();
	for (; iter != techniques.end(); ++iter) {
		iter->second->Link(callback);
	}
}

void TGen::Material::setSpecialization(const std::string & name, TGen::TechniqueList * techniques) {
	TechniqueListMap::iterator iter = this->techniques.find(getSpecializationID(name));
	if (iter != this->techniques.end())
		delete iter->second;
	
	this->techniques[getSpecializationID(name)] = techniques;
}

void TGen::Material::Render(TGen::Renderer & renderer, TGen::Renderable & renderable, const std::string & mode, int lod) {
	TGen::TechniqueList * techniques = NULL;
	int specialization = getSpecializationID(mode);
	
	TechniqueListMap::iterator iter = this->techniques.find(specialization);
	if (iter == this->techniques.end())
		throw TGen::RuntimeException("Material::Render", "material doesn't contain specialization \"" + mode + "\"");
	
	techniques = iter->second;
	
	TGen::Technique * technique = techniques->getTechnique(minimumTechnique);
	if (!technique)
		throw TGen::RuntimeException("Material::Render", "no techniques");
	
	TGen::PassList * passes = technique->getPassList(lod);
	if (!passes)
		throw TGen::RuntimeException("Material::Render", "no passes");
	
	passes->Render(renderer, renderable);
}

void TGen::Material::Update(scalar time) {
	TechniqueListMap::iterator iter = techniques.begin();
	for (; iter != techniques.end(); ++iter) {
		iter->second->Update(time);
	}
}

int TGen::Material::getSpecializationID(const std::string & name) {
	SpecializationMap::iterator iter = specializations.find(name);
	if (iter == specializations.end()) {
		int newId = specializations.size() + 1;
		specializations[name] = newId;
		return newId;
	}
	
	return iter->second;
}

void TGen::Material::setMaximumTechnique(int minreqs)  {
	minimumTechnique = minreqs;
}

void TGen::Material::setParameter(const std::string & name, const std::vector<std::string> & values) {
	parameters[name] = values;
}

std::vector<std::string> & TGen::Material::getParameter(const std::string & name) {
	return parameters[name];
}



