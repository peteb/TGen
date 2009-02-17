/*
 *  subsystem.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/24/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "info/subsystem.h"
#include "info/worldinfo.h"
#include "world.h"

TGen::Engine::Info::Subsystem::Subsystem()
	: world(NULL)
{
}

TGen::Engine::Component * TGen::Engine::Info::Subsystem::createComponent(const std::string & name, TGen::Engine::Entity & entity, const TGen::PropertyTree & properties)
{
	TGen::Engine::Component * ret = NULL;

	if (properties.getName() == "worldInfo")
		ret = createWorldInfo(name, properties);
	else
		throw TGen::RuntimeException("Info::Subsystem::createComponent", "invalid component type for " + name);
	
	return ret;
}

TGen::Engine::Info::WorldInfo * TGen::Engine::Info::Subsystem::createWorldInfo(const std::string & name, const TGen::PropertyTree & properties) {
	TGen::Engine::Info::WorldInfo * newInfo = new TGen::Engine::Info::WorldInfo(name, world);
	
	newInfo->setPlayerController(properties.getProperty("playerController", ""));
	newInfo->setAmbientLight(TGen::Color::Parse(properties.getProperty("ambientLight", "0 0 0")));
	
	if (!world)
		throw TGen::RuntimeException("Info::Subsystem::createWorldInfo", "no world linked!");
	
	world->setWorldInfo(newInfo);
	
	return newInfo;
}

void TGen::Engine::Info::Subsystem::setWorld(TGen::Engine::World * world) {
	this->world = world;
}
