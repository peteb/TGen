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
#include "player.h"

Level::Level(World & world) : nicePlane(NULL), world(world) {
	
}

Level::~Level() {
	delete nicePlane;
}

void Level::Render(TGen::Renderer & renderer) {
	if (nicePlane) {
		// detta ska flyttas upp minst en nivå
		
		
		TGen::Vector3 forward, up, right;
		forward = world.getPlayer()->getDirection().getNormalized();
		up = TGen::Vector3(0.0f, -1.0f, 0.0f);
		right = TGen::Vector3::CrossProduct(forward, up).Normalize();
		
		TGen::Matrix4x4 at = TGen::Matrix4x4::Rotation(TGen::Vector3(0.0f, 1.0f, 0.0f), TGen::Radian(-world.getPlayer()->lookDir)) * TGen::Matrix4x4::Translation(world.getPlayer()->getPosition() + TGen::Vector3(0.0f, -3.0f, 0.0f));
		
		TGen::Matrix4x4 & mat = nicePlane->getTransform(renderer);
		renderer.setTransform(TGen::TransformWorldView, at * nicePlane->getTransform(renderer));
		nicePlane->getMaterial()->Render(renderer, *nicePlane, "default", 9, NULL);
	}
}

void Level::Update(scalar dt) {
	
}

void Level::LoadData(TGen::Renderer & renderer, ResourceManager & resources) {
	delete nicePlane;
	nicePlane = world.CreatePlane(TGen::Vector3(0.0f, 0.0f, 0.0f), TGen::Vector3(0.0f, -1.0f, 0.0f), resources.getMaterial("tilefloor7"));	
	nicePlane->LoadData(renderer, resources);

}
