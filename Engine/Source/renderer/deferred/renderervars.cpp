/*
 *  deferredrenderervars.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 11/20/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "renderer/deferred/renderervars.h"
#include "app.h"
#include "variableregister.h"

TGen::Engine::DeferredRendererVars::DeferredRendererVars(TGen::Engine::VariableRegister & variables) 
	: variables(variables)
	, forceBinaryMRT(true)
	, postBloom(true)
	, postProcessing(true)
	, bloomBlurPasses(2)
	, bloomDownsampling(1)
	, lumTrace(true)
	, lumMin(0.7)
	, lumMultiplier(0.7)
{
	variables.addVariable(TGen::Engine::Variable("r_forceBinaryMRT", "true", "true", 0), TGen::Engine::AddOverrideDefaults | TGen::Engine::AddNoThrow);
	variables.addVariable(TGen::Engine::Variable("r_postBloom", "true", "true", 0), TGen::Engine::AddOverrideDefaults | TGen::Engine::AddNoThrow);
	variables.addVariable(TGen::Engine::Variable("r_postBloomBlurPasses", "2", "2", 0), TGen::Engine::AddOverrideDefaults | TGen::Engine::AddNoThrow);
	variables.addVariable(TGen::Engine::Variable("r_postBloomDownsampling", "1", "1", 0), TGen::Engine::AddOverrideDefaults | TGen::Engine::AddNoThrow);
	variables.addVariable(TGen::Engine::Variable("r_postLuminanceMin", "0.9", "0.35", 0), TGen::Engine::AddOverrideDefaults | TGen::Engine::AddNoThrow);
	variables.addVariable(TGen::Engine::Variable("r_postLuminanceMultiplier", "0.9", "0.6", 0), TGen::Engine::AddOverrideDefaults | TGen::Engine::AddNoThrow);
	variables.addVariable(TGen::Engine::Variable("r_postLuminanceTrace", "false", "false", 0), TGen::Engine::AddOverrideDefaults | TGen::Engine::AddNoThrow);

	variables["r_forceBinaryMRT"].addObserver(this);
	variables["r_postBloom"].addObserver(this);
	variables["r_postBloomBlurPasses"].addObserver(this);
	variables["r_postBloomDownsampling"].addObserver(this);
	variables["r_postLuminanceMin"].addObserver(this);
	variables["r_postLuminanceMultiplier"].addObserver(this);
	variables["r_postLuminanceTrace"].addObserver(this);
	
	loadVariables();	
}

void TGen::Engine::DeferredRendererVars::postVariableChange(const TGen::Engine::Variable & variable) {
	loadVariables();	
}

void TGen::Engine::DeferredRendererVars::onVariableRemoved(const TGen::Engine::Variable & variable) {
	
}

void TGen::Engine::DeferredRendererVars::loadVariables() {
	forceBinaryMRT = bool(variables["r_forceBinaryMRT"]);
	postBloom = bool(variables["r_postBloom"]);
	bloomBlurPasses = int(variables["r_postBloomBlurPasses"]);
	bloomDownsampling = int(variables["r_postBloomDownsampling"]);
	lumTrace = bool(variables["r_postLuminanceTrace"]);
	lumMin = scalar(variables["r_postLuminanceMin"]);
	lumMultiplier = scalar(variables["r_postLuminanceMultiplier"]);
	
	postProcessing = postBloom;
}

