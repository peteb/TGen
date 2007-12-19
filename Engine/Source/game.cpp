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
#include "world.h"


TGen::Engine::GameState::GameState(TGen::Engine::App & app)
	: TGen::Engine::State(app)
	, lastRender(TGen::Time::Now())
	, currentWorld(NULL)
	, sceneRenderer(NULL)
	, lastErrorCheck(0.0)
	, vars(NULL)
{
	app.logs.info["gst+"] << "entering game state..." << endl;
	
	try {
		sceneRenderer = new TGen::Engine::DeferredRenderer(app);
	}
	catch (const std::exception & e) {
		app.logs.error["gst+"] << "failed to create deferred renderer: \"" << e.what() << "\" and there is no fallback! Go ask peter for a fallback." << TGen::endl;
		throw;
	}
	
	vars = new TGen::Engine::GameStateVars(app, this);
	if (!vars)
		throw TGen::RuntimeException("GameState::GameState", "failed to create game state vars...");
}

TGen::Engine::GameState::~GameState() {
	delete currentWorld;
	delete vars;
	
	app.logs.info["gst-"] << "leaving game state..." << endl;
}

void TGen::Engine::GameState::tick() {
	TGen::Time now = TGen::Time::Now();
	double sinceLastRender = double(now) - double(lastRender);
	lastErrorCheck += sinceLastRender;
	
	if (sinceLastRender >= vars->maxRefreshInterval) {
		lastRender = now;
		
		if (currentWorld)
			currentWorld->update(sinceLastRender);
		
		render(sinceLastRender);
	//	std::cout << 1.0 / sinceLastRender << std::endl;
		
	}
	else {
		if (vars->conserveCPU && sinceLastRender < vars->maxRefreshInterval / 2.0)	// we don't want to cause irregular render updates
			TGen::Sleep(TGen::Time(sinceLastRender));
	}
}

void TGen::Engine::GameState::render(scalar dt) {
	sceneRenderer->renderScene(dt);
	app.env.swapBuffers();
	
	if (vars->checkErrors)
		checkErrors();
}

void TGen::Engine::GameState::checkErrors() {
	if (lastErrorCheck >= 1.0) {
		lastErrorCheck = 0.0;
		
		while (1) {
			try {
				if (!app.renderer.throwErrors())
					break;
			}
			catch (const TGen::RuntimeException & e) {
				app.logs.warning["game"] << "Renderer issued error last second: \"" << e.getDescription() << "\"." << TGen::endl;
			}
		}
	}	
}

void TGen::Engine::GameState::changeMap(const std::string & mapName) {
	app.logs.info["game"] << "changing map to '" << mapName << "'..." << TGen::endl;

	delete currentWorld;
	currentWorld = new TGen::Engine::World(app, mapName);
	
	if (sceneRenderer)
		sceneRenderer->setWorld(currentWorld);
}


