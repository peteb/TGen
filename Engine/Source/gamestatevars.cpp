/*
 *  gamestatevars.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 10/8/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "gamestatevars.h"
#include "app.h"
#include "variablesregistry.h"

TGen::Engine::GameStateVars::GameStateVars(TGen::Engine::App & app) 
	: app(app)	
	, maxRefreshInterval(0.0f)
	, syncVtrace(false)	
	, conserveCPU(true)
{
	app.variables.addVariable(TGen::Engine::Variable("r_maxRefresh", "40", "-1", TGen::Engine::VariableDump), false);
	app.variables.addVariable(TGen::Engine::Variable("r_syncVtrace", "false", "false", TGen::Engine::VariableDump), false);
	app.variables.addVariable(TGen::Engine::Variable("r_conserveCPU", "true", "true", TGen::Engine::VariableDump), false);
	
	app.variables["r_maxRefresh"].addObserver(this);
	app.variables["r_syncVtrace"].addObserver(this);
	app.variables["r_conserveCPU"].addObserver(this);
	
	loadVariables();
}

void TGen::Engine::GameStateVars::postVariableChange(const TGen::Engine::Variable & variable) {
	app.logs.info["game"] << "variable '" << variable.getName() << "' changed" << TGen::endl;
	loadVariables();
}

void TGen::Engine::GameStateVars::onVariableRemoved(const TGen::Engine::Variable & variable) {
	
}

void TGen::Engine::GameStateVars::loadVariables() {
	scalar r_maxRefresh = scalar(app.variables["r_maxRefresh"]);
	
	if (r_maxRefresh >= 0.0f)
		maxRefreshInterval = (1000.0f / r_maxRefresh) / 1000.0f;
	else
		maxRefreshInterval = 0.0f;
	
	syncVtrace = bool(app.variables["r_syncVtrace"]);
	conserveCPU = bool(app.variables["r_conserveCPU"]);
	
	app.logs.info["game"] << "maxRefreshInterval: " << maxRefreshInterval << TGen::endl;
	app.logs.info["game"] << "syncVtrace: " << syncVtrace << TGen::endl;
	app.logs.info["game"] << "conserveCPU: " << conserveCPU << TGen::endl;
}
