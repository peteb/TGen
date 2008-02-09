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

TGen::Engine::GameStateVars::GameStateVars(TGen::Engine::App & app, TGen::Engine::GameState * state) 
	: app(app)	
	, state(state)
	, maxRefreshInterval(0.0f)
	, syncVtrace(false)	
	, conserveCPU(true)
	, multithread(false)
	, checkErrors(true)
{
	app.variables.addVariable(TGen::Engine::Variable("r_maxRefresh", "40", "-1", TGen::Engine::VariableDump), TGen::Engine::AddOverrideDefaults | TGen::Engine::AddNoThrow);
	app.variables.addVariable(TGen::Engine::Variable("r_syncVtrace", "true", "false", TGen::Engine::VariableDump), TGen::Engine::AddOverrideDefaults | TGen::Engine::AddNoThrow);
	app.variables.addVariable(TGen::Engine::Variable("r_conserveCPU", "true", "true", TGen::Engine::VariableDump), TGen::Engine::AddOverrideDefaults | TGen::Engine::AddNoThrow);
	app.variables.addVariable(TGen::Engine::Variable("r_multithread", "false", "false", TGen::Engine::VariableDump), TGen::Engine::AddOverrideDefaults | TGen::Engine::AddNoThrow);
	app.variables.addVariable(TGen::Engine::Variable("r_checkErrors", "true", "true", TGen::Engine::VariableDump), TGen::Engine::AddOverrideDefaults | TGen::Engine::AddNoThrow);
	app.variables.addVariable(TGen::Engine::Variable("map", "testmap", "testmap", 0), TGen::Engine::AddOverrideDefaults | TGen::Engine::AddNoThrow);

	app.variables["map"].addObserver(this);
	app.variables["r_checkErrors"].addObserver(this);
	app.variables["r_multithread"].addObserver(this);
	app.variables["r_maxRefresh"].addObserver(this);
	app.variables["r_syncVtrace"].addObserver(this);
	app.variables["r_conserveCPU"].addObserver(this);
	// TODO: scenenoden verkar inte räkna ut global orientation korrekt... fixa + ändra till vector3 för orientation
	
	loadVariables();
}

void TGen::Engine::GameStateVars::postVariableChange(const TGen::Engine::Variable & variable) {
	app.logs.info["game"] << "variable '" << variable.getName() << "' changed" << TGen::endl;
	loadVariables();
}

void TGen::Engine::GameStateVars::onVariableRemoved(const TGen::Engine::Variable & variable) {
	
}

// normalmappad modell: xyz, uv, n, t	// coord texcoord (colormap, nmap, specmap, glowmap, etc) normal tangent
// tråkig modell: xyz, uv, n

/*
	Conserve CPU minimerar antalet extraupdates som görs om grafikkortet är snabbt nog att rendrera maxRefreshRate
	På en snabb dator kan det betyda att processorn används mindre
 */

void TGen::Engine::GameStateVars::loadVariables() {
	scalar r_maxRefresh = scalar(app.variables["r_maxRefresh"]);
	
	if (r_maxRefresh >= 0.0f)
		maxRefreshInterval = (1000.0f / r_maxRefresh) / 1000.0f;
	else
		maxRefreshInterval = 0.0f;
	
	syncVtrace = bool(app.variables["r_syncVtrace"]);
	conserveCPU = bool(app.variables["r_conserveCPU"]);
	multithread = bool(app.variables["r_multithread"]);
	checkErrors = bool(app.variables["r_checkErrors"]);
	
	/*app.logs.info["rend"] << "maxRefreshInterval: " << maxRefreshInterval << TGen::endl;
	app.logs.info["rend"] << "syncVtrace: " << syncVtrace << TGen::endl;
	app.logs.info["rend"] << "conserveCPU: " << conserveCPU << TGen::endl;
	*/
	
	if (app.variables["map"].getValue() != mapName) {
		mapName = app.variables["map"].getValue();
		app.logs.info["game"] << "map changed to " << mapName << TGen::endl;
		
		if (state)
			state->changeMap(mapName);
	}
}

