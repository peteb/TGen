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
#include "variableregister.h"
#include "renderer/rendererfactory.h"
#include "renderer/worldrenderer.h"

#include <iostream>

TGen::Engine::App::App(TGen::Engine::VariableRegister & variables, TGen::Engine::CommandRegister & commands, TGen::Engine::Environment & env, 
					   TGen::Engine::Filesystem * fs, const TGen::PropertyTree & props, TGen::Renderer & renderer, 
					   TGen::Engine::StandardLogs & logs, TGen::Engine::DeviceCollection & inputDevices)
	: running(true)
	, startedAt(TGen::Time::Now())
	, currentState(NULL)
	, worldRenderer(NULL)
	, env(env)
	, variables(variables)
	, commands(commands)
	, filesystem(*fs)
	, loadProps(props)
	, renderer(renderer)
	, inputDevices(inputDevices)
	, logs(logs)
	, globalResources(logs, filesystem, renderer)
{
	logs.info["app+"] << "initializing..." << TGen::endl;
	
	
	// när man skapar en modell ska man skicka med en omfräsare som kan parsas från gensträngen
	/*
	 ops: scale, swap
	 
	 VertexManipulator
		vec3 manipulate(vec3 in);
	 
	 createModel(VertexManipulator);
	 
	 VertexManipulatorList : VertexManipulator
	 VertexScaler : VertexManipulator
	 VertexSwapper : VertexManipulator
	 VertexTransformer : VertexManipulator
	 
	 VertexScaler(vec3(1.0f, 2.0f, 1.0f));
	 VertexSwapper(VertexSwapper::X, VertexSwapper::Y);
	 
	 
	 */
	
	TGen::Engine::WorldRendererFactory rendererFactory(logs);
	
	// TODO: man ser ju hur många referenser varje grej har... borde kunna lösas upp en hel del
	worldRenderer = rendererFactory.createRenderer(variables["r_renderer"].getValue(), renderer, logs, variables, globalResources);
	currentState = new TGen::Engine::GameState(inputDevices, env, filesystem, variables, logs, *worldRenderer, globalResources, renderer);
	
	logs.info["app+"] << "initialized" << TGen::endl;
}


TGen::Engine::App::~App() {
	logs.info["app-"] << "shutting down..." << TGen::endl;
	
	delete currentState;
	delete worldRenderer;
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
	
	globalResources.updateMaterials(TGen::Time::Now() - startedAt);
}

