/*
 *  physicssubsystem.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 2/29/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "physicssubsystem.h"
#include "log.h"
#include "bodycomponent.h"

TGen::Engine::PhysicsSubsystem::PhysicsSubsystem(TGen::Engine::StandardLogs & logs) 
	: logs(logs)
{
	logs.info["phys+"] << "*** INITIALIZING PHYSICS ***" << TGen::endl;
}

TGen::Engine::PhysicsSubsystem::~PhysicsSubsystem() {
	logs.info["phys-"] << "*** SHUTTING DOWN PHYSICS ***" << TGen::endl;
}

TGen::Engine::Component * TGen::Engine::PhysicsSubsystem::createComponent(TGen::Engine::Entity * entity, const TGen::PropertyTree & properties) {
	if (properties.getName() == "physBody") {
		return createBody(properties);
	}
	
	return NULL;
}

TGen::Engine::BodyComponent * TGen::Engine::PhysicsSubsystem::createBody(const TGen::PropertyTree & properties) {
	float mass = TGen::lexical_cast<float>(properties.getProperty("mass", "1.0"));
	TGen::Vector3 position = TGen::Vector3::Parse(properties.getProperty("position", "0 0 0"));
	
	TGen::Engine::BodyComponent * newBody = new TGen::Engine::BodyComponent("physBody", mass, position);
	bodies.push_back(newBody);
	
	return newBody;
}

void TGen::Engine::PhysicsSubsystem::link() {
	// RESOLVA ALLA REFERENSER TILL ANDRA KOMPONENTER
}

void TGen::Engine::PhysicsSubsystem::update(scalar delta) {
	for (int i = 0; i < bodies.size(); ++i) {
		bodies[i]->update(delta); //position += 0.01f * delta;
	}
}
