/*
 *  level.cpp
 *  The Place
 *
 *  Created by Peter Backman on 7/9/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "level.h"
#include "resourcemanager.h"
#include "world.h"

Level::Level(World & world) : nicePlane(NULL), world(world) {
	
}

Level::~Level() {
	delete nicePlane;
}

void Level::Render(TGen::Renderer & renderer) {
	if (nicePlane) {
		nicePlane->ApplyTransform(renderer);
		nicePlane->getMaterial()->Render(renderer, *nicePlane, "default", 9);
	}
}

void Level::Update(scalar dt) {
	
}

void Level::LoadData(TGen::Renderer & renderer, ResourceManager & resources) {
	delete nicePlane;
	nicePlane = world.CreatePlane(TGen::Vector3(0.0f, 0.0f, 0.0f), TGen::Vector3(0.0f, 1.0f, 0.0f), resources.getMaterial("tilefloor7"));	
	nicePlane->LoadData(renderer, resources);
}
