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
#include "jointcomponent.h"
#include <ode/ode.h>
#include <tgen_math.h>

TGen::Engine::PhysicsSubsystem::PhysicsSubsystem(TGen::Engine::StandardLogs & logs) 
	: logs(logs)
	, updateInterval(0.02)
{
	logs.info["phys+"] << "*** INITIALIZING PHYSICS ***" << TGen::endl;
	
	worldId = dWorldCreate();
	
	setGravity(TGen::Vector3(0.0f, -9.0f, 0.0f));
}

TGen::Engine::PhysicsSubsystem::~PhysicsSubsystem() {
	logs.info["phys-"] << "*** SHUTTING DOWN PHYSICS ***" << TGen::endl;
	
	dCloseODE();
}

TGen::Engine::Component * TGen::Engine::PhysicsSubsystem::createComponent(TGen::Engine::Entity * entity, const TGen::PropertyTree & properties) {
	if (properties.getName() == "physBody")
		return createBody(properties);
	else if (properties.getName() == "physJoint")
		return createJoint(properties);
	
	return NULL;
}

TGen::Engine::BodyComponent * TGen::Engine::PhysicsSubsystem::createBody(const TGen::PropertyTree & properties) {
	float mass = TGen::lexical_cast<float>(properties.getProperty("mass", "1.0"));
	TGen::Vector3 position = TGen::Vector3::Parse(properties.getProperty("position", "0 0 0"));
	bool applyGravity = TGen::lexical_cast<bool>(properties.getProperty("gravity", "true"));
	
	
	dBodyID newBodyId = dBodyCreate(worldId);
	
	dBodySetGravityMode(newBodyId, applyGravity);
	
	TGen::Engine::BodyComponent * newBody = new TGen::Engine::BodyComponent("physBody", newBodyId);
	bodies.push_back(newBody);
	
	return newBody;
}

TGen::Engine::JointComponent * TGen::Engine::PhysicsSubsystem::createJoint(const TGen::PropertyTree & properties) {
	if (properties.getNumAttributes() == 0)
		throw TGen::RuntimeException("PhysicsSubsystem::createJoint", "no attributes, plz give some");
	
	std::string jointType = properties.getAttribute(0);
	
	dJointID newJointId;
	
	if (jointType == "ball")
		newJointId = dJointCreateBall(worldId, 0);
	else
		throw TGen::RuntimeException("PhysicsSubsystem::createJoint", "joint type '" + jointType + "' invalid");
	
	TGen::Engine::JointComponent * newComponent = new TGen::Engine::JointComponent("physJoint", newJointId, properties.getProperty("attach", ""));
	
	newComponent->setAnchor(TGen::Vector3::Parse(properties.getProperty("anchor", "0 0 0")));
	
	
	return newComponent;
}

void TGen::Engine::PhysicsSubsystem::link() {
	
}

void TGen::Engine::PhysicsSubsystem::update(scalar dt) {
	static float sumDeltas = 0.0f;
	sumDeltas += dt;
	
	if (sumDeltas > updateInterval) {
		sumDeltas -= updateInterval;
		
		for (int i = 0; i < bodies.size(); ++i)
			bodies[i]->preStep();
		
		dWorldStep(worldId, updateInterval); // tweak
		
		for (int i = 0; i < bodies.size(); ++i)
			bodies[i]->postStep();
	}
}

void TGen::Engine::PhysicsSubsystem::setGravity(const TGen::Vector3 & gravity) {
	dWorldSetGravity(worldId, gravity.x, gravity.y, gravity.z);
}