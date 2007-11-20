/*
 *  deferredrenderervars.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 11/20/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "deferredrenderervars.h"
#include "app.h"
#include "variablesregistry.h"

TGen::Engine::DeferredRendererVars::DeferredRendererVars(TGen::Engine::App & app) 
	: app(app)
	, postBloom(true)
	, postProcessing(true)
{
	app.variables.addVariable(TGen::Engine::Variable("r_postBloom", "true", "true", 0), TGen::Engine::AddOverrideDefaults | TGen::Engine::AddNoThrow);
	
	app.variables["r_postBloom"].addObserver(this);
	
	loadVariables();	
}

void TGen::Engine::DeferredRendererVars::postVariableChange(const TGen::Engine::Variable & variable) {
	loadVariables();	
}

void TGen::Engine::DeferredRendererVars::onVariableRemoved(const TGen::Engine::Variable & variable) {
	
}

void TGen::Engine::DeferredRendererVars::loadVariables() {
	postBloom = bool(app.variables["r_postBloom"]);
	
	postProcessing = postBloom;
}

