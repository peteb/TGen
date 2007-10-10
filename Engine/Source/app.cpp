/*
 *  app.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/27/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "app.h"
#include "state.h"
#include "game.h"
#include <iostream>

TGen::Engine::App::App(TGen::Engine::VariablesRegistry & variables, TGen::Engine::Environment & env, 
					   TGen::Engine::Filesystem * fs, const TGen::PropertyTree & props, TGen::Renderer & renderer, 
					   TGen::Engine::StandardLogs & logs)
	: running(true)
	, currentState(NULL)
	, env(env)
	, variables(variables)
	, filesystem(*fs)
	, loadProps(props)
	, renderer(renderer)
	, logs(logs)
{
	logs.info["app+"] << "initializing..." << TGen::endl;
		
	currentState = new TGen::Engine::GameState(*this);
	
	logs.info["app+"] << "initialized" << TGen::endl;
}


TGen::Engine::App::~App() {
	delete currentState;
	logs.info["app-"] << "shut down" << TGen::endl;
}

bool TGen::Engine::App::isRunning() const {
	return running;
}

void TGen::Engine::App::quit() {
	running = false;
}

void TGen::Engine::App::tick() {
	if (currentState)
		currentState->tick();
}

