/*
 *  game.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 10/8/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "game.h"
#include "variableregister.h"
#include <iostream>
#include "environment.h"
#include "world.h"
#include "devicecollection.h"
#include "log.h"

TGen::Engine::GameState::GameState(TGen::Engine::DeviceCollection & inputDevices, TGen::Engine::Environment & env, TGen::Engine::Filesystem & filesystem, TGen::Engine::VariableRegister & variables, TGen::Engine::StandardLogs & logs, TGen::Engine::WorldRenderer & worldRenderer, TGen::Engine::ResourceManager & resources, TGen::VertexDataSource & dataSource)
	: logs(logs)
	, inputDevices(inputDevices)
	, env(env)
	, filesystem(filesystem)
	, resources(resources)
	, dataSource(dataSource)
	, constructed(false)
	, lastRender(TGen::Time::Now())
	, currentWorld(NULL)
	, sinceErrorCheck(0.0)
	, worldRenderer(worldRenderer)
	, vars(variables, logs, *this)
	, player(NULL)
{
	logs.info["gst+"] << "entering game state..." << endl;
	
	if (!throttledNewMap.empty()) {
		std::cout << "a throttled map!" << std::endl;
		currentWorld = new TGen::Engine::World(filesystem, resources, logs, worldRenderer.getRenderer(), throttledNewMap);
		inputMapper.setWorld(currentWorld);
		throttledNewMap = "";
		postWorldCreation(*currentWorld);
	}

	inputDevices.enterMode(TGen::Engine::DefaultMode);
	inputDevices.enterMode(TGen::Engine::RelativeMode);
	
	constructed = true;
}

TGen::Engine::GameState::~GameState() {
	inputDevices.enterMode(TGen::Engine::DefaultMode);
	
	delete currentWorld;
	
	logs.info["gst-"] << "leaving game state..." << endl;
}

void TGen::Engine::GameState::tick() {
	inputDevices.dispatchEvents(inputMapper);
	
	TGen::Time now = TGen::Time::Now();
	double sinceLastRender = double(now) - double(lastRender);	// TODO: undersök om scalar kanske räcker, sen operator - på Time. det här suger.
	sinceErrorCheck += sinceLastRender;
	
	if (sinceLastRender >= vars.maxRefreshInterval) {
		lastRender = now;
		
		//playerController.update(sinceLastRender);
		
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

// glBufferData lite som discard, tänk på det. det är samma grej som med lock fast bufferSubData
// en megabuffer som man vet kommer uppdateras totalt borde discardas först

void TGen::Engine::GameState::render(scalar dt) {
	if (currentWorld)
		worldRenderer.renderWorld(*currentWorld, dt);
	
	env.swapBuffers();
	//std::cout << "statistics this frame: " << std::endl << std::string(app.renderer.getStatistics()) << std::endl;
	
	worldRenderer.getRenderer().getStatistics().reset();
	
	if (vars.checkErrors)
		checkErrors();
}

void TGen::Engine::GameState::checkErrors() {
	if (sinceErrorCheck >= 2.0) {
		sinceErrorCheck = 0.0;
		
		while (1) {
			try {
				if (!worldRenderer.getRenderer().throwErrors())
					break;
			}
			catch (const TGen::RuntimeException & e) {
				logs.warning["game"] << "Renderer issued error: \"" << e.getDescription() << "\"." << TGen::endl;
			}
		}
	}	
}

// TODO: ska kanske player vara i world ändå?

void TGen::Engine::GameState::postWorldCreation(TGen::Engine::World & world) {
	//player = world.createPlayer();
	inputMapper.setPlayerController(world.getPlayerController("player_start"));
}

void TGen::Engine::GameState::changeMap(const std::string & mapName) {
	if (constructed) {
		logs.info["game"] << "changing map to '" << mapName << "'..." << TGen::endl;

		delete currentWorld;
		currentWorld = new TGen::Engine::World(filesystem, resources, logs, dataSource, mapName);
		
		inputMapper.setWorld(currentWorld);
		postWorldCreation(*currentWorld);
	}
	else {
		throttledNewMap = mapName;
	}
}


