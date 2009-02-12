/*
 *  worldinfo.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/24/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "info/worldinfo.h"
#include "playercontroller.h"
#include "world.h"

TGen::Engine::Info::WorldInfo::WorldInfo(const std::string & name, TGen::Engine::World * world)
	: TGen::Engine::Component(name)
	, world(world)
{
}

void TGen::Engine::Info::WorldInfo::link(const TGen::Engine::ComponentLinker & linker) {
	playerController.link(linker);
}

void TGen::Engine::Info::WorldInfo::setPlayerController(const std::string & name) {
	playerController.set(name);
}

TGen::Engine::PlayerController * TGen::Engine::Info::WorldInfo::getPlayerController() {
	if (!playerController)
		return NULL;
	
	return &*playerController;
}

void TGen::Engine::Info::WorldInfo::setAmbientLight(const TGen::Color & light) {
	this->ambientLight = light;
}

TGen::Color TGen::Engine::Info::WorldInfo::getAmbientLight() const {
	return ambientLight;
}

