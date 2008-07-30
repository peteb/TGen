/*
 *  eventoperation.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/12/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "eventoperation.h"
#include "entity.h"

TGen::Engine::Script::EventOperation::EventOperation(EventOperation * parent) 
	: parent(parent)
	, numParameters(0)
	, numLocalVars(0)
{
	
}

TGen::Engine::Script::EventOperation::~EventOperation() {
	for (int i = 0; i < operations.size(); ++i)
		delete operations[i];
	
}

void TGen::Engine::Script::EventOperation::addOperation(EventOperation * operation) {
	operations.push_back(operation);
}

void TGen::Engine::Script::EventOperation::trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode) {
	for (int i = 0; i < operations.size(); ++i)
		operations[i]->trigger(context, mode);
}

void TGen::Engine::Script::EventOperation::link(const TGen::Engine::ComponentLinker & linker) {
	for (int i = 0; i < operations.size(); ++i)
		operations[i]->link(linker);
}

void TGen::Engine::Script::EventOperation::linkRecipe(const TGen::Engine::EntityRecipe & recipe) {
	for (int i = 0; i < operations.size(); ++i)
		operations[i]->linkRecipe(recipe);
}

void TGen::Engine::Script::EventOperation::addAlias(const std::string & alias, const std::string & value) {
	aliases.insert(std::make_pair(alias, value));
}

std::string TGen::Engine::Script::EventOperation::getAlias(const std::string & alias) const {
	AliasMap::const_iterator iter = aliases.find(alias);
	if (iter == aliases.end()) {
		if (parent)
			return parent->getAlias(alias);
		else
			return "";
	}
	
	return iter->second;
}

void TGen::Engine::Script::EventOperation::setNumParameters(int num) {
	numParameters = num;
}

int TGen::Engine::Script::EventOperation::getNumParameters() const {
	return numParameters;
}

void TGen::Engine::Script::EventOperation::setNumLocalVars(int localVars) {
	this->numLocalVars = localVars;
}

int TGen::Engine::Script::EventOperation::getNumLocalVars() const {
	return numLocalVars;
}

