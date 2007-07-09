/*
 *  world.cpp
 *  The Place
 *
 *  Created by Peter Backman on 7/9/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "world.h"
#include <iostream>



World::World() {
	std::cout << "[world]: initializing..." << std::endl;
	
	gravity = TGen::Vector3(0.0f, -9.82, 0.0f);
	
	worldId = dWorldCreate();
	std::cout << "[ode]: created world " << worldId << std::endl;
	
	dWorldSetGravity(worldId, gravity.x, gravity.y, gravity.z);
	dWorldSetAutoDisableFlag(worldId, true);
	
	std::cout << "[world]:   gravity = " << std::string(gravity) << std::endl;
	
}

World::~World() {
	std::cout << "[world]: shutting down..." << std::endl;
	
	dWorldDestroy(worldId);
}

void World::Update() {
	dWorldQuickStep(worldId, 10);
}

void World::Render() {
	
}

