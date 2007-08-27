/*
 *  app.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/27/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "app.h"

TGen::Engine::App::App()
	: running(true)
{
}

TGen::Engine::App::~App() {
	
}

bool TGen::Engine::App::isRunning() const {
	return running;
}

void TGen::Engine::App::quit() {
	running = false;
}

void TGen::Engine::App::tick() {
	
}

