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
#include "player.h"

World::World() : lastWorldStep(0.0), timeNow(0.0), player(NULL) {
	std::cout << "[world]: initializing..." << std::endl;
	
	gravity = TGen::Vector3(0.0f, 0.00982f, 0.0f);
	
	worldId = dWorldCreate();
	std::cout << "[ode]: created world " << worldId << std::endl;
	
	contactJointsGroupId = dJointGroupCreate(0);
	
	dReal center[3] = {0.0f, 0.0f, 0.0f};
	dReal extents[3] = {200.0f, 200.0f, 200.0f};

	spaceId = dQuadTreeSpaceCreate(NULL, center, extents, 7);

	
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
		dSpaceCollide(spaceId, this, NearCallback);	// TODO: space
		//dWorldQuickStep(worldId, 1);
		dWorldStep(worldId, 1);
		dJointGroupEmpty(contactJointsGroupId);

	}

	if (level)
		level->Update(dt);
	
	if (player)
		player->Update(dt);
}

void World::Render(TGen::Renderer & renderer) {
	if (level)
		level->Render(renderer);
}

void World::LoadData(TGen::Renderer & renderer, ResourceManager & resources) {
	if (level)
		level->LoadData(renderer, resources);
	
	player = CreatePlayer(TGen::Vector3(0.0f, -4.0f, 0.0f), TGen::Vector3(0.0f, 0.0f, 1.0f));
}

Plane * World::CreatePlane(const TGen::Vector3 & position, const TGen::Vector3 & direction, TGen::Material * material) {
	dGeomID newId = dCreatePlane(spaceId, direction.x, direction.y, direction.z, -position.getMagnitude());
	//dGeomSetPosition(newId, position.x, position.y, position.z);
	
	return new Plane(position, direction, material, newId);
}

Player * World::CreatePlayer(const TGen::Vector3 & position, const TGen::Vector3 & direction) {
	dBodyID bodyId = dBodyCreate(worldId);
	dMass mass;
	dMassSetCapsule(&mass, 1.0, 2, 2.0, 6.0);
	dMassAdjust(&mass, 0.2f);
	dBodySetMass(bodyId, &mass);
	
	dBodySetPosition(bodyId, position.x, position.y, position.z);
	dGeomID geomId = dCreateSphere(spaceId, 4.0f); //dCreateBox(spaceId, 5.0f, 15.0f, 5.0f); //dCreateCapsule(spaceId, 2.0, 6.0);
	
	//TGen::Matrix4x4 rot = TGen::Matrix4x4::Rotation(TGen::Vector3(1.0f, 0.0f, 0.0f), TGen::Degree(90));
	
	dMatrix3 hej;
//	dRFromAxisAndAngle(hej, 0.0f, 0.0f, 1.0f, TGen::PI / 2.0);
	
//	dGeomSetRotation(geomId, hej);
	dGeomSetBody(geomId, bodyId);
	
	return new Player(position, direction, geomId, bodyId);
}

Player * World::getPlayer() {
	return player;
}

void NearCallback(void * data, dGeomID o1, dGeomID o2) {
	World * world = reinterpret_cast<World *>(data);
	
	dContactGeom contact;
	memset(&contact, 0, sizeof(dContactGeom));
	dCollide(o1, o2, 1, &contact, sizeof(dContactGeom));
	
	//if (contact.depth > 0.0f && dGeomGetBody(o1) && dGeomGetBody(o2))
	//	cout << "hit!" << endl;
	
	if (contact.depth <= 0.0f)
		return;
	
	dContact contactInfo;
	memset(&contactInfo, 0, sizeof(dContact));
	contactInfo.geom = contact;
	contactInfo.surface.mode = 0;
	contactInfo.surface.mu = dInfinity / dInfinity / 10.0f;
	
	dJointID contactJoint = dJointCreateContact(world->worldId, world->contactJointsGroupId, &contactInfo);
	dJointAttach(contactJoint, dGeomGetBody(o1), dGeomGetBody(o2));
	
}