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
#include "planegeomcomponent.h"
#include "spheregeomcomponent.h"
#include <ode/ode.h>
#include <tgen_math.h>

dWorldID TGen::Engine::PhysicsSubsystem::worldId = 0;
dJointGroupID TGen::Engine::PhysicsSubsystem::contactGroup = 0;

TGen::Engine::PhysicsSubsystem::PhysicsSubsystem(TGen::Engine::StandardLogs & logs) 
	: logs(logs)
	, updateInterval(0.02)
{
	logs.info["phys+"] << "*** INITIALIZING PHYSICS ***" << TGen::endl;
	
	worldId = dWorldCreate();
	mainSpace = dSimpleSpaceCreate(0);
	contactGroup = dJointGroupCreate(0);
	
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
	else if (properties.getName() == "physGeom")
		return createGeom(properties);
	
	throw TGen::RuntimeException("PhysicsSubsystem::createComponent", "invalid component type '" + properties.getName() + "'");
}

TGen::Engine::BodyComponent * TGen::Engine::PhysicsSubsystem::createBody(const TGen::PropertyTree & properties) {
	float mass = TGen::lexical_cast<float>(properties.getProperty("mass", "1.0"));
	TGen::Vector3 position = TGen::Vector3::Parse(properties.getProperty("position", "0 0 0"));
	bool applyGravity = TGen::lexical_cast<bool>(properties.getProperty("gravity", "true"));
	
	
	dBodyID newBodyId = dBodyCreate(worldId);
	
	dBodySetGravityMode(newBodyId, applyGravity);
	
	TGen::Engine::BodyComponent * newBody = new TGen::Engine::BodyComponent("physBody", newBodyId);
	newBody->setPosition(position);
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

TGen::Engine::Component * TGen::Engine::PhysicsSubsystem::createGeom(const TGen::PropertyTree & properties) {
	if (properties.getNumAttributes() == 0)
		throw TGen::RuntimeException("PhysicsSubsystem::createJoint", "no attributes, plz give some");

	std::string geomType = properties.getAttribute(0);

	TGen::Engine::Component * newComponent = NULL;
	
	if (geomType == "plane") {
		TGen::Vector3 normal = TGen::Vector3::Parse(properties.getProperty("orientation", "0 1 0"));
		scalar distance = TGen::lexical_cast<scalar>(properties.getProperty("distance", "0"));
	
		newComponent = new TGen::Engine::PlaneGeomComponent("physGeom", TGen::Plane3(normal, distance), mainSpace);
	}
	else if (geomType == "sphere") {
		scalar radius = TGen::lexical_cast<scalar>(properties.getProperty("radius", "1.0"));
		
		newComponent = new TGen::Engine::SphereGeomComponent("physGeom", radius, mainSpace);
	}
	else {
		throw TGen::RuntimeException("PhysicsSubsystem::createGeom", "invalid geom type '" + geomType + "'!");
	}
	
	
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
		
		dSpaceCollide(mainSpace, 0, &nearCallback);

		dWorldStep(worldId, updateInterval); // tweak

		dJointGroupEmpty(contactGroup);
		
		for (int i = 0; i < bodies.size(); ++i)
			bodies[i]->postStep();
	}
}

void TGen::Engine::PhysicsSubsystem::setGravity(const TGen::Vector3 & gravity) {
	dWorldSetGravity(worldId, gravity.x, gravity.y, gravity.z);
}

void TGen::Engine::PhysicsSubsystem::nearCallback(void * data, dGeomID o1, dGeomID o2) {
	if (dGeomIsSpace(o1) || dGeomIsSpace(o2)) {
		dSpaceCollide2(o1, o2, data, &nearCallback);
		
		//if (dGeomIsSpace(o1))
		//	dSpaceCollide(o1, data, &nearCallback);
		//if (dGeomIsSpace(o2))
		//	dSpaceCollide(o2, data, &nearCallback);
	}
	else {
		const int MAX_CONTACTS = 20;
		dContact contactGeoms[MAX_CONTACTS];
		int numContacts = dCollide(o1, o2, MAX_CONTACTS, &contactGeoms[0].geom, sizeof(dContact));
		
		dBodyID body1 = dGeomGetBody(o1);
		dBodyID body2 = dGeomGetBody(o2);
		
		for (int i = 0; i < numContacts; ++i) {
			dJointID contactJoint = dJointCreateContact(worldId, contactGroup, &contactGeoms[i]);
			dJointAttach(contactJoint, body1, body2);
		}
	}
	
}

