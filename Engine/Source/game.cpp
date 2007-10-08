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
	, lastRender(TGen::Time::Now())
	, sceneRoot("root")
	, vars(app)
{
	std::cout << "[gam]: entering game state..." << std::endl;
	app.info.output("gam", "hej");
}

TGen::Engine::GameState::~GameState() {
	std::cout << "[gam]: leaving game state..." << std::endl;
}


void TGen::Engine::GameState::tick() {
	TGen::Time now = TGen::Time::Now();
	double sinceLastRender = double(now) - double(lastRender);
	
	if (sinceLastRender >= vars.maxRefreshInterval) {
		lastRender = now;
		render(sinceLastRender);
	}
	else {
		if (vars.conserveCPU && sinceLastRender < vars.maxRefreshInterval / 2.0)	// we don't want to cause irregular render times
			TGen::Sleep(sinceLastRender);
	}
	
	
}


void TGen::Engine::GameState::render(scalar dt) {
	//std::cout << "dt: " << dt << std::endl;

	
	app.renderer.clearBuffers(TGen::ColorBuffer);
	app.env.swapBuffers();
}

