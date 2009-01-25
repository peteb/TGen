/*
 *  renderervars.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 1/25/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include "renderer/forward/renderervars.h"
#include "variableregister.h"

TGen::Engine::ForwardRendererVars::ForwardRendererVars(TGen::Engine::VariableRegister & variables)
	: variables(variables)
{
	variables.addVariable(Variable("r_optShadowScissor", "false", "false", 0), AddOverrideDefaults | AddNoThrow);
	
	loadSettings();
}

void TGen::Engine::ForwardRendererVars::loadSettings() {
	settings.optShadowScissor = bool(variables["r_optShadowScissor"]);

}

const TGen::Engine::ForwardRendererSettings & TGen::Engine::ForwardRendererVars::getSettings() const {
	return settings;
}
