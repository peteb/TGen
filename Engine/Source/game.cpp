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
	, constructed(false)
	, lastRender(TGen::Time::Now())
	, currentWorld(NULL)
	, sceneRenderer(NULL)
	, sinceErrorCheck(0.0)
	, vars(app, this)
{
	app.logs.info["gst+"] << "entering game state..." << endl;
	// TODO: faktiskt bättre att ha vars som objekt istället, sen om man försöker sätta banan innan det constructorn är utförd så laddas banan i slutet
	try {
		sceneRenderer = new TGen::Engine::DeferredRenderer(app);
	}
	catch (const std::exception & e) {
		app.logs.error["gst+"] << "failed to create deferred renderer: \"" << e.what() << "\" and there is no fallback! Go ask peter for a fallback." << TGen::endl;
		throw;
	}
	
	if (!throttledNewMap.empty()) {
		currentWorld = new TGen::Engine::World(app, throttledNewMap);
		sceneRenderer->setWorld(currentWorld);
		throttledNewMap = "";
	}

	constructed = true;
}

TGen::Engine::GameState::~GameState() {
	delete currentWorld;
	
	app.logs.info["gst-"] << "leaving game state..." << endl;
}

void TGen::Engine::GameState::tick() {
	TGen::Time now = TGen::Time::Now();
	double sinceLastRender = double(now) - double(lastRender);	// TODO: undersök om scalar kanske räcker, sen operator - på Time
	sinceErrorCheck += sinceLastRender;
	
	if (sinceLastRender >= vars.maxRefreshInterval) {
		lastRender = now;
		
		if (currentWorld)
			currentWorld->update(sinceLastRender);
		
		render(sinceLastRender);
	//	std::cout << 1.0 / sinceLastRender << std::endl;
		
	}
	else {
		if (vars.conserveCPU && sinceLastRender < vars.maxRefreshInterval / 2.0)	// we don't want to cause irregular render updates
			TGen::Sleep(TGen::Time(sinceLastRender));
	}
}

void TGen::Engine::GameState::render(scalar dt) {
	sceneRenderer->renderScene(dt);
	app.env.swapBuffers();
	std::cout << "statistics this frame: " << std::endl << std::string(app.renderer.getStatistics()) << std::endl;
	
	app.renderer.getStatistics().reset();
	
	if (vars.checkErrors)
		checkErrors();
}

void TGen::Engine::GameState::checkErrors() {
	if (sinceErrorCheck >= 2.0) {
		sinceErrorCheck = 0.0;
		
		while (1) {
			try {
				if (!app.renderer.throwErrors())
					break;
			}
			catch (const TGen::RuntimeException & e) {
				app.logs.warning["game"] << "Renderer issued error: \"" << e.getDescription() << "\"." << TGen::endl;
			}
		}
	}	
}

//       sen ska shaderpreprocess.cpp byta namn till textpreprocessor

void TGen::Engine::GameState::changeMap(const std::string & mapName) {
	app.logs.info["game"] << "changing map to '" << mapName << "'..." << TGen::endl;

	if (constructed) {
		delete currentWorld;
		currentWorld = new TGen::Engine::World(app, mapName);
	
		if (!sceneRenderer)
			throw TGen::RuntimeException("GameState::changeMap", "dude, where's my scene renderer!");
		
		sceneRenderer->setWorld(currentWorld);
	}
	else {
		throttledNewMap = mapName;
	}
}


