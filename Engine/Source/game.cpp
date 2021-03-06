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
#include "playercontroller.h"
#include "resourcemanager.h"

TGen::Engine::GameState::GameState(TGen::Engine::DeviceCollection & inputDevices, TGen::Engine::Environment & env, TGen::Engine::Filesystem & filesystem, TGen::Engine::VariableRegister & variables, TGen::Engine::StandardLogs & logs, TGen::Engine::WorldRenderer & worldRenderer, TGen::Engine::ResourceManager & resources, TGen::VertexDataSource & dataSource)
	: logs(logs)
	, inputDevices(inputDevices)
	, env(env)
	, filesystem(filesystem)
	, resources(resources)
	, dataSource(dataSource)
	, constructed(false)
	, lastRender(TGen::Time::Now())
	, lastUpdate(TGen::Time::Now())
	, currentWorld(NULL)
	, sinceErrorCheck(0.0)
	, worldRenderer(worldRenderer)
	, vars(variables, logs, *this)
	, player(NULL)
{
	logs.info["gst+"] << "entering game state..." << endl;
	
	//if (!throttledNewMap.empty()) {			// TODO: uncommenting this will make it possible to start a world without the level setting
		std::cout << "a throttled map!" << std::endl;
		
	
		
		// heavy, show some graphics.
		// just start with a simple fading green screen or something, multithreaded
		// then interleave with GUI somehow.
		currentWorld = new TGen::Engine::World(filesystem, resources, logs, resources.vertexCache, throttledNewMap);		// throttledNewMap
		
		// logs = shared, mutex?   renderer = shared, mutex?
		
		
		// TODO: change map in script! stringRef. [world changeLevel: @stringRef]
		
		throttledNewMap = "";
		postWorldCreation(*currentWorld);
	
	
	//}

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
	
	// gör om det här, render kallas varje tick medans physics bara lite då och då, typ 40 fps
	
	TGen::Time now = TGen::Time::Now();
	double sinceLastRender = double(now) - double(lastRender);
	//double sinceLastUpdate = double(now) - double(lastUpdate);
	
	if (currentWorld && currentWorld->getPlayerController()) {
		TGen::Vector3 playerPosition, playerVelocity, playerForward, playerUp;
		
		TGen::Engine::WorldObject * viewObject = currentWorld->getPlayerController();
		if (!viewObject)
			viewObject = currentWorld->getPlayerCamera();
		
		TGenAssert(viewObject);
		
 		TGen::Matrix4x4 invertedCam = TGen::Matrix4x4(viewObject->getOrientation()).getInverse();	// TODO: improve! TGen::Rotation without mat4
		
		
		
		if (viewObject) {
			playerPosition = viewObject->getPosition();
			playerVelocity = viewObject->getVelocity();
		}
		
		playerForward = invertedCam.getZ();
		playerUp = invertedCam.getY();
		
		currentWorld->updateListener(playerPosition, playerVelocity, playerForward, playerUp);		
		currentWorld->update(sinceLastRender);	
	}
	
	render(sinceLastRender);
	lastRender = now;
}

// glBufferData lite som discard, tänk på det. det är samma grej som med lock fast bufferSubData
// en megabuffer som man vet kommer uppdateras totalt borde discardas först

void TGen::Engine::GameState::render(scalar dt) {
	static scalar sumTime = 0.0f;
	sumTime += dt;
	
	/*if (sumTime > 15.0f && sumTime < 20.0f) {
		sumTime = 100.0f;
		try {
			changeMap("subwaymap");
		}
		catch (...) {
			logs.error["game"] << "failed to change map" << TGen::endl;
		}
	}*/
	
	if (currentWorld && currentWorld->getPlayerCamera()) 
		worldRenderer.renderWorld(*currentWorld, dynamic_cast<TGen::Camera *>(currentWorld->getPlayerCamera()->getSceneNode()), dt);
	
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
	inputDevices.reset();

	
	inputMapper.setPlayerController(world.getPlayerController());
}

void TGen::Engine::GameState::changeMap(const std::string & mapName) {
	if (constructed) {
		logs.info["game"] << "changing map to '" << mapName << "'..." << TGen::endl;

		// do the multithreading here... it shouldn't be done elsewhere
		// a currentWorld pointer, and a nextWorld pointer. and a nextConstructed bool. (with mutex)
		// when nextConstructed, set currentWorld to nextWorld and remove lastWorld
		inputDevices.reset();

		TGen::Engine::World * newWorld = new TGen::Engine::World(filesystem, resources, logs, dataSource, mapName);
	
		delete currentWorld;
		currentWorld = newWorld;
		
		postWorldCreation(*currentWorld);
	}
	else {
		throttledNewMap = mapName;
	}
}


