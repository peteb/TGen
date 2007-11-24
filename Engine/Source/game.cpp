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
	, world(app)
	, sceneRenderer(NULL)
{
	app.logs.info["gst+"] << "entering game state..." << endl;
	
	try {
		sceneRenderer = new TGen::Engine::DeferredRenderer(app, world);
	}
	catch (const std::exception & e) {
		app.logs.error["gst+"] << "failed to create deferred renderer: \"" << e.what() << "\" and there is no fallback!" << TGen::endl;
		throw;
	}
}

TGen::Engine::GameState::~GameState() {
	app.logs.info["gst-"] << "leaving game state..." << endl;
}

void TGen::Engine::GameState::tick() {
	TGen::Time now = TGen::Time::Now();
	double sinceLastRender = double(now) - double(lastRender);
	
	if (sinceLastRender >= vars.maxRefreshInterval) {
		lastRender = now;
		world.update(sinceLastRender);
		render(sinceLastRender);
		//std::cout << 1.0 / sinceLastRender << std::endl;
		// dags att undersöka lite fps
	}
	else {
		if (vars.conserveCPU && sinceLastRender < vars.maxRefreshInterval / 2.0)	// we don't want to cause irregular render updates
			TGen::Sleep(TGen::Time(sinceLastRender));
	}
	
}

// fixa world::update här
// DeferredSceneRenderer vars

void TGen::Engine::GameState::render(scalar dt) {
	sceneRenderer->renderScene(dt);
	
	app.env.swapBuffers();
}

