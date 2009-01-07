/*
 *  shadervarupdater.cpp
 *  TGen Graphics
 *
 *  Created by Peter Backman on 1/7/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include "shadervarupdater.h"

TGen::ShaderUpdater::ShaderUpdater(const std::string & varName, ShaderVarType type, float value) 
	: varName(varName)
	, linkedVar(NULL)
	, type(type)
	, value(value) 
{
}

TGen::ShaderUpdater::~ShaderUpdater() {
	delete linkedVar;
	// TODO: linkedVar should be deleted, but then it won't work in all the other updaters
}

TGen::ShaderUpdater::ShaderUpdater(const ShaderUpdater & other) 
	: linkedVar(NULL)
	, varName(other.varName)
	, type(other.type)
	, value(other.value)
{
}

void TGen::ShaderUpdater::link(TGen::ShaderProgram * shader) {
	linkedVar = shader->createVariable(varName);
}

TGen::ShaderUpdater * TGen::ShaderUpdater::clone() const {
	return new TGen::ShaderUpdater(*this);
}

void TGen::ShaderUpdater::update() {
	if (linkedVar) {
		(*linkedVar) = int(value);
	}
}
