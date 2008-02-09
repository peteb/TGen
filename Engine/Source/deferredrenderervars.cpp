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
#include "variableregister.h"

TGen::Engine::DeferredRendererVars::DeferredRendererVars(TGen::Engine::App & app) 
	: app(app)
	, forceBinaryMRT(true)
	, postBloom(true)
	, postProcessing(true)
	, bloomBlurPasses(2)
	, bloomDownsampling(2)
	, lumTrace(true)
	, lumMin(0.7)
	, lumMultiplier(0.7)
{
	app.variables.addVariable(TGen::Engine::Variable("r_forceBinaryMRT", "true", "true", 0), TGen::Engine::AddOverrideDefaults | TGen::Engine::AddNoThrow);
	app.variables.addVariable(TGen::Engine::Variable("r_postBloom", "true", "true", 0), TGen::Engine::AddOverrideDefaults | TGen::Engine::AddNoThrow);
	app.variables.addVariable(TGen::Engine::Variable("r_postBloomBlurPasses", "3", "3", 0), TGen::Engine::AddOverrideDefaults | TGen::Engine::AddNoThrow);
	app.variables.addVariable(TGen::Engine::Variable("r_postBloomDownsampling", "2", "2", 0), TGen::Engine::AddOverrideDefaults | TGen::Engine::AddNoThrow);
	app.variables.addVariable(TGen::Engine::Variable("r_postLuminanceMin", "0.9", "0.35", 0), TGen::Engine::AddOverrideDefaults | TGen::Engine::AddNoThrow);
	app.variables.addVariable(TGen::Engine::Variable("r_postLuminanceMultiplier", "0.9", "0.6", 0), TGen::Engine::AddOverrideDefaults | TGen::Engine::AddNoThrow);
	app.variables.addVariable(TGen::Engine::Variable("r_postLuminanceTrace", "false", "false", 0), TGen::Engine::AddOverrideDefaults | TGen::Engine::AddNoThrow);

	app.variables["r_forceBinaryMRT"].addObserver(this);
	app.variables["r_postBloom"].addObserver(this);
	app.variables["r_postBloomBlurPasses"].addObserver(this);
	app.variables["r_postBloomDownsampling"].addObserver(this);
	app.variables["r_postLuminanceMin"].addObserver(this);
	app.variables["r_postLuminanceMultiplier"].addObserver(this);
	app.variables["r_postLuminanceTrace"].addObserver(this);
	
	loadVariables();	
}

void TGen::Engine::DeferredRendererVars::postVariableChange(const TGen::Engine::Variable & variable) {
	loadVariables();	
}

void TGen::Engine::DeferredRendererVars::onVariableRemoved(const TGen::Engine::Variable & variable) {
	
}

void TGen::Engine::DeferredRendererVars::loadVariables() {
	forceBinaryMRT = bool(app.variables["r_forceBinaryMRT"]);
	postBloom = bool(app.variables["r_postBloom"]);
	bloomBlurPasses = int(app.variables["r_postBloomBlurPasses"]);
	bloomDownsampling = int(app.variables["r_postBloomDownsampling"]);
	lumTrace = bool(app.variables["r_postLuminanceTrace"]);
	lumMin = scalar(app.variables["r_postLuminanceMin"]);
	lumMultiplier = scalar(app.variables["r_postLuminanceMultiplier"]);
	
	postProcessing = postBloom;
}

