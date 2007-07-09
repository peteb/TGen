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



World::World() : lastWorldStep(0.0), timeNow(0.0) {
	std::cout << "[world]: initializing..." << std::endl;
	
	gravity = TGen::Vector3(0.0f, -9.82, 0.0f);
	
	worldId = dWorldCreate();
	std::cout << "[ode]: created world " << worldId << std::endl;
	
	dWorldSetGravity(worldId, gravity.x, gravity.y, gravity.z);
	dWorldSetAutoDisableFlag(worldId, true);
	
	std::cout << "[world]:   gravity = " << std::string(gravity) << std::endl;

	level = new Level(*this);
}

World::~World() {
	std::cout << "[world]: shutting down..." << std::endl;

	delete level;
	dWorldDestroy(worldId);
}

void World::Update(scalar dt) {
	timeNow += dt;
	
	if (timeNow - lastWorldStep >= 0.02) {
		lastWorldStep = timeNow;
		dWorldQuickStep(worldId, 10);
	}
	
	if (level)
		level->Update(dt);
}

void World::Render(TGen::Renderer & renderer) {
	if (level)
		level->Render(renderer);
}

void World::LoadData(TGen::Renderer & renderer, ResourceManager & resources) {
	if (level)
		level->LoadData(renderer, resources);
}

Plane * World::CreatePlane(const TGen::Vector3 & position, const TGen::Vector3 & direction, TGen::Material * material) {
	dGeomID newId = dCreatePlane(0, direction.x, direction.y, direction.z, position.getMagnitude());	// TODO: better space partitioning
	//dGeomSetPosition(newId, position.x, position.y, position.z);
	
	return new Plane(position, direction, material, newId);
}
