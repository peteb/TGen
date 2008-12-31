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
#include "techniquelist.h"
#include "passlist.h"

//TGen::Material::SpecializationMap TGen::Material::specializations;
TGen::SymbolTable TGen::Material::specializations;

TGen::Material::Material(const std::string & name) 
	: minimumTechnique(9)
	, sortLevel(TGen::MaterialSortOpaque)    // 0-19 = front to back 20 - 39 = back to front    
	, name(name) 
	, linked(false)
	, timesRendered(0)
{
	/*TGen::TechniqueList * newTechniques = new TGen::TechniqueList;
	TGen::Technique * newTechnique = new TGen::Technique;
	TGen::PassList * passes = new TGen::PassList;
	TGen::Pass * pass = new TGen::Pass;
	
	passes->addPass(pass);
	newTechnique->setPassList(passes, 0);
	newTechniques->setTechnique(newTechnique, 0);
	
	
	techniques[getSpecializationID("default")] = newTechniques;*/
}

TGen::Material::~Material() {
	
}

std::string TGen::Material::getName() const {
	return name;
}

void TGen::Material::link(TGen::MaterialLinkCallback & callback) {
	TechniqueListMap::iterator iter = techniques.begin();
	for (; iter != techniques.end(); ++iter) {
		iter->second->link(callback);
	}
	
	linked = true;
}

bool TGen::Material::isLinked() const {
	return linked;
}

void TGen::Material::setSpecialization(const std::string & name, TGen::TechniqueList * techniques) {
	TechniqueListMap::iterator iter = this->techniques.find(specializations[name]);
	if (iter != this->techniques.end())
		delete iter->second;
	
	this->techniques[specializations[name]] = techniques;
}

/*void TGen::Material::render(TGen::Renderer & renderer, const TGen::Renderable & renderable, const std::string & mode, int lod, TGen::Texture ** textureTypes, TGen::ShaderVariableUpdater * varupdater, TGen::MaterialOverride * override) {
	TGen::Technique * technique = getSpecialization(mode);
	
	TGen::PassList * passes = technique->getPassList(lod);
	if (!passes) throw TGen::RuntimeException("Material::Render", "no passes");
	
	passes->render(renderer, renderable, textureTypes, varupdater, override);
	timesRendered++;
}*/


void TGen::Material::render(TGen::Renderer & renderer, const TGen::Renderable & renderable, const MaterialRenderMetadata & metadata) {
	TGen::Technique * technique = getSpecialization(metadata.mode);
	TGen::PassList * passes = technique->getPassList(metadata.lod);
	
	if (!passes) 
		throw TGen::RuntimeException("Material::Render", "no passes");
	
	passes->render(renderer, renderable, metadata);
	timesRendered++;	
}

TGen::Technique * TGen::Material::getSpecialization(const std::string & name) {
	TGen::TechniqueList * techniques = NULL;
	int specialization = specializations[name];
	
	TechniqueListMap::iterator iter = this->techniques.find(specialization);	// TODO: man ska kunna ange specnum direkt också
	if (iter == this->techniques.end()) {
	//	std::cout << "warning: material doesn't contain specialization \"" << name << "\" using 'default'" << std::endl;
		iter = this->techniques.find(specializations["default"]);
		
		if (iter == this->techniques.end())
			throw TGen::RuntimeException("Material::getSpecialization", "material doesn't contain specialization \"" + name + "\" and no 'default'!");
	}
	techniques = iter->second;
	
	TGen::Technique * technique = techniques->getTechnique(minimumTechnique);
	if (!technique) throw TGen::RuntimeException("Material::getSpecialization", "no techniques");
	
	return technique;
}

TGen::TechniqueList * TGen::Material::getTechniqueList(const std::string & name) {
	int spec = specializations[name];
	
	TechniqueListMap::iterator iter = this->techniques.find(spec);
	if (iter != this->techniques.end())
		return iter->second;
	
	return NULL;
}

const TGen::Material::SpecializationList & TGen::Material::getSpecializations() const {
	return techniques;
}

void TGen::Material::update(scalar time) {
	TechniqueListMap::iterator iter = techniques.begin();
	for (; iter != techniques.end(); ++iter) {
		iter->second->update(time);
	}
}

/*int TGen::Material::getSpecializationID(const std::string & name) {
	SpecializationMap::iterator iter = specializations.find(name);
	if (iter == specializations.end()) {
		int newId = specializations.size() + 1;
		specializations[name] = newId;
		return newId;
	}
	
	return iter->second;
}*/

void TGen::Material::setMaximumTechnique(int minreqs)  {
	minimumTechnique = minreqs;
}

void TGen::Material::setParameter(const std::string & name, const std::vector<std::string> & values) {
	parameters[name] = values;
}

std::vector<std::string> & TGen::Material::getParameter(const std::string & name) {
	return parameters[name];
}

int TGen::Material::getTimesRendered() const {
	return timesRendered;
}

/*void TGen::Material::setSortLevel(int level) {
	sortLevel = level;	// render: 001234555555556789
}

int TGen::Material::getSortLevel() const {
	return sortLevel;
}


*/




TGen::MaterialRenderMetadata::MaterialRenderMetadata(const std::string & mode, int lod, TGen::Texture ** textureTypes, int shaderMode, TGen::ShaderVariableUpdater * varupdater, TGen::MaterialOverride * override)
	: mode(mode)
	, lod(lod)
	, textureTypes(textureTypes)
	, shaderMode(shaderMode)
	, varupdater(varupdater)
	, override(override)
{
	
}




