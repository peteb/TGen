/*
 *  variable.cpp
 *  variableregister
 *
 *  Created by Peter Backman on 8/9/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "variable.h"
#include <algorithm>

TGen::Engine::Variable::Variable(const std::string & name, const std::string & value, const std::string & defaultValue, uint flags)
	: name(name)
	, value(value)
	, defaultValue(defaultValue)
	, flags(flags)
{
	observers.reserve(1);	
}

TGen::Engine::Variable::~Variable() {
	triggerRemoved();
}

TGen::Engine::VariableChangeRejected::VariableChangeRejected(const Variable & variable, const std::string & description)
	: TGen::RuntimeException(variable.getName(), description)
{		
}

TGen::Engine::VariableObserver::~VariableObserver() {
	for (int i = 0; i < variablesObserving.size(); ++i) {
		variablesObserving[i]->removeObserver(this);
	}
}

void TGen::Engine::VariableObserver::addVariableObserved(Variable * variable) {
	variablesObserving.push_back(variable);
}

void TGen::Engine::Variable::addObserver(VariableObserver * observer) {
	if (std::find(observers.begin(), observers.end(), observer) != observers.end())
		return;
	
	observers.push_back(observer);
	observer->addVariableObserved(this);
}

void TGen::Engine::Variable::removeObserver(VariableObserver * observer) {
	ObserverList::iterator iter = std::find(observers.begin(), observers.end(), observer);
	if (iter != observers.end())
		observers.erase(iter);
}

void TGen::Engine::Variable::triggerPreChange(const std::string & newValue) {
	for (int i = 0; i < observers.size(); ++i) {
		observers[i]->preVariableChange(*this, newValue);
	}
}

void TGen::Engine::Variable::triggerPostChange() {
	for (int i = 0; i < observers.size(); ++i) {
		observers[i]->postVariableChange(*this);
	}	
}

void TGen::Engine::Variable::triggerRemoved() {
	for (int i = 0; i < observers.size(); ++i) {
		observers[i]->onVariableRemoved(*this);
	}	
}

std::string TGen::Engine::Variable::getName() const {
	return name;
}

std::string TGen::Engine::Variable::getDefaultValue() const {
	return defaultValue;
}

uint TGen::Engine::Variable::getFlags() const {
	return flags;
}

void TGen::Engine::Variable::takeDefaults(const TGen::Engine::Variable & var) {
	defaultValue = var.getDefaultValue();
	flags = var.getFlags();
}

														