/*
 *  variablesregistry.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/27/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "variablesregistry.h"
#include <tgen_core.h>

TGen::Engine::VariablesRegistry::VariablesRegistry() {
	
}

TGen::Engine::VariablesRegistry::~VariablesRegistry() {
	
}

const TGen::Engine::Variable & TGen::Engine::VariablesRegistry::getVariable(const std::string & name) const {
	VariableMap::const_iterator iter = variables.find(name);
	
	if (iter == variables.end())
		throw TGen::RuntimeException("VariablesRegistry::getVariable", "variable '" + name + "' does not exist");
	
	return iter->second;
}

TGen::Engine::Variable & TGen::Engine::VariablesRegistry::getVariable(const std::string & name) {
	VariableMap::iterator iter = variables.find(name);
	
	if (iter == variables.end())
		throw TGen::RuntimeException("VariablesRegistry::getVariable", "variable '" + name + "' does not exist");
	
	return iter->second;	
}

void TGen::Engine::VariablesRegistry::addVariable(const TGen::Engine::Variable & variable, bool doThrow) {
	VariableMap::const_iterator iter = variables.find(variable.getName());
	
	if (iter != variables.end()) {	// TODO: check whether the variable has a "remove on new"-flag
		if (doThrow)
			throw TGen::RuntimeException("VariablesRegistry::addVariable", "variable '" + variable.getName() + "' already exists");
		else
			return;
	}
	
	variables.insert(VariableMap::value_type(variable.getName(), variable));
}

const TGen::Engine::Variable & TGen::Engine::VariablesRegistry::operator [] (const std::string & name) const {
	return getVariable(name);
}

TGen::Engine::Variable & TGen::Engine::VariablesRegistry::operator [] (const std::string & name) {
	return getVariable(name);
}

