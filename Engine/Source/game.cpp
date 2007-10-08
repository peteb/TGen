/*
 *  game.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 10/8/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "game.h"
#include "app.h"
#include "variablesregistry.h"
#include <iostream>
#include "sdl.h"

TGen::Engine::GameState::GameState(TGen::Engine::App & app)
	: TGen::Engine::State(app)
	, maxRefreshInterval(0.0f)
	, syncVtrace(false)
	, lastRender(TGen::Time::Now())
	, sceneRoot("root")
{
	std::cout << "[gam]: entering game state..." << std::endl;

	app.variables.addVariable(TGen::Engine::Variable("r_maxRefresh", "30", "-1", TGen::Engine::VariableDump), false);
	app.variables.addVariable(TGen::Engine::Variable("r_syncVtrace", "false", "false", TGen::Engine::VariableDump), false);

	app.variables.getVariable("r_maxRefresh").addObserver(this);
	app.variables.getVariable("r_syncVtrace").addObserver(this);
	
	loadVariables();
}

TGen::Engine::GameState::~GameState() {
	std::cout << "[gam]: leaving game state..." << std::endl;
}

void TGen::Engine::GameState::preVariableChange(const TGen::Engine::Variable & variable, const std::string & newValue) {
	loadVariables();
}

void TGen::Engine::GameState::onVariableRemoved(const TGen::Engine::Variable & variable) {
	
}

void TGen::Engine::GameState::loadVariables() {
	scalar r_maxRefresh = scalar(app.variables["r_maxRefresh"]);
	
	if (r_maxRefresh >= 0.0f)
		maxRefreshInterval = (1000.0f / r_maxRefresh) / 1000.0f;
	else
		maxRefreshInterval = 0.0f;
	
	syncVtrace = bool(app.variables["r_syncVtrace"]);
	
	std::cout << "[gam]: maxRefreshInterval: " << maxRefreshInterval << std::endl;
	std::cout << "[gam]: syncVtrace: " << syncVtrace << std::endl;
}

void TGen::Engine::GameState::tick() {
	TGen::Time now = TGen::Time::Now();
	double sinceLastRender = double(now) - double(lastRender);
	if (sinceLastRender >= maxRefreshInterval) {
		lastRender = now;
		render(sinceLastRender);
	}
	
}


void TGen::Engine::GameState::render(scalar dt) {
	std::cout << "dt: " << dt << std::endl;
	
	app.renderer.clearBuffers(TGen::ColorBuffer);
	app.env.swapBuffers();
}

