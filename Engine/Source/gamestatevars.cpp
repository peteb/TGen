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
#include "variableregister.h"
#include "game.h"

TGen::Engine::GameStateVars::GameStateVars(TGen::Engine::VariableRegister & variables, TGen::Engine::StandardLogs & logs, TGen::Engine::GameState & state) 
	: variables(variables)	
	, logs(logs)
	, state(state)
	, maxRefreshInterval(0.0f)
	, syncVtrace(false)	
	, conserveCPU(true)
	, multithread(false)
	, checkErrors(true)
{
	variables.addVariable(TGen::Engine::Variable("r_maxRefresh", "40", "-1", TGen::Engine::VariableDump), TGen::Engine::AddOverrideDefaults | TGen::Engine::AddNoThrow);
	variables.addVariable(TGen::Engine::Variable("r_syncVtrace", "true", "false", TGen::Engine::VariableDump), TGen::Engine::AddOverrideDefaults | TGen::Engine::AddNoThrow);
	variables.addVariable(TGen::Engine::Variable("r_conserveCPU", "true", "true", TGen::Engine::VariableDump), TGen::Engine::AddOverrideDefaults | TGen::Engine::AddNoThrow);
	variables.addVariable(TGen::Engine::Variable("r_multithread", "false", "false", TGen::Engine::VariableDump), TGen::Engine::AddOverrideDefaults | TGen::Engine::AddNoThrow);
	variables.addVariable(TGen::Engine::Variable("r_checkErrors", "true", "true", TGen::Engine::VariableDump), TGen::Engine::AddOverrideDefaults | TGen::Engine::AddNoThrow);
	variables.addVariable(TGen::Engine::Variable("map", "testmap", "testmap", 0), TGen::Engine::AddOverrideDefaults | TGen::Engine::AddNoThrow);

	variables["map"].addObserver(this);
	variables["r_checkErrors"].addObserver(this);
	variables["r_multithread"].addObserver(this);
	variables["r_maxRefresh"].addObserver(this);
	variables["r_syncVtrace"].addObserver(this);
	variables["r_conserveCPU"].addObserver(this);
	
	loadVariables();
}

void TGen::Engine::GameStateVars::postVariableChange(const TGen::Engine::Variable & variable) {
	logs.info["game"] << "variable '" << variable.getName() << "' changed" << TGen::endl;
	loadVariables();
}

void TGen::Engine::GameStateVars::onVariableRemoved(const TGen::Engine::Variable & variable) {
	
}

/*
	Conserve CPU minimerar antalet extraupdates som görs om grafikkortet är snabbt nog att rendrera maxRefreshRate
	På en snabb dator kan det betyda att processorn används mindre
 */

void TGen::Engine::GameStateVars::loadVariables() {
	scalar r_maxRefresh = scalar(variables["r_maxRefresh"]);
	
	if (r_maxRefresh >= 0.0f)
		maxRefreshInterval = (1000.0f / r_maxRefresh) / 1000.0f;
	else
		maxRefreshInterval = 0.0f;
	
	syncVtrace = bool(variables["r_syncVtrace"]);
	conserveCPU = bool(variables["r_conserveCPU"]);
	multithread = bool(variables["r_multithread"]);
	checkErrors = bool(variables["r_checkErrors"]);
	
	/*app.logs.info["rend"] << "maxRefreshInterval: " << maxRefreshInterval << TGen::endl;
	app.logs.info["rend"] << "syncVtrace: " << syncVtrace << TGen::endl;
	app.logs.info["rend"] << "conserveCPU: " << conserveCPU << TGen::endl;
	*/
	
	if (variables["map"].getValue() != mapName) {
		mapName = variables["map"].getValue();
		logs.info["game"] << "map changed to " << mapName << TGen::endl;
		
		
		//if (state)
			state.changeMap(mapName);
	}
}

