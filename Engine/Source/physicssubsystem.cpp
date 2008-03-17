/*
 *  physicssubsystem.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 2/29/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "physics/subsystem.h"
#include "log.h"
#include "bodycomponent.h"
#include "jointcomponent.h"
#include "planegeomcomponent.h"
#include "spheregeomcomponent.h"
#include "boxgeomcomponent.h"
#include "geomcomponent.h"
#include <ode/ode.h>
#include <tgen_math.h>

dWorldID TGen::Engine::Physics::Subsystem::worldId = 0;
dJointGroupID TGen::Engine::Physics::Subsystem::contactGroup = 0;

TGen::Engine::Physics::Subsystem::Subsystem(TGen::Engine::StandardLogs & logs) 
	: logs(logs)
	, updateInterval(0.02)
{
	logs.info["phys+"] << "*** INITIALIZING PHYSICS ***" << TGen::endl;
	
	worldId = dWorldCreate();
	mainSpace = dSimpleSpaceCreate(0);
	contactGroup = dJointGroupCreate(0);
	
	setGravity(TGen::Vector3(0.0f, -10.0f, 0.0f));
}

TGen::Engine::Physics::Subsystem::~Subsystem() {
	logs.info["phys-"] << "*** SHUTTING DOWN PHYSICS ***" << TGen::endl;
	
	dCloseODE();
}

TGen::Engine::Component * TGen::Engine::Physics::Subsystem::createComponent(TGen::Engine::Entity * entity, const TGen::PropertyTree & properties) {
	if (properties.getName() == "physBody")
		return createBody(properties);
	else if (properties.getName() == "physJoint")
		return createJoint(properties);
	else if (properties.getName() == "physGeom")
		return createGeom(properties);
	else if (properties.getName() == "physProps")
		return NULL; //createProps(properties);
	
	throw TGen::RuntimeException("PhysicsSubsystem::createComponent", "invalid component type '" + properties.getName() + "'");
}

TGen::Engine::BodyComponent * TGen::Engine::Physics::Subsystem::createBody(const TGen::PropertyTree & properties) {
	//float mass = TGen::lexical_cast<float>(properties.getProperty("mass", "1.0"));
	TGen::Vector3 position = TGen::Vector3::Parse(properties.getProperty("position", "0 0 0"));
	bool applyGravity = TGen::lexical_cast<bool>(properties.getProperty("gravity", "true"));
	
	dBodyID newBodyId = dBodyCreate(worldId);
	dBodySetGravityMode(newBodyId, applyGravity);
	
	try {
		std::string massType = properties.getNode("mass").getAttribute(0);
		
		dMass mass;
		
		if (massType == "box") {
			scalar totalMass = TGen::lexical_cast<scalar>(properties.getNode("mass").getProperty("total", "1.0"));
			TGen::Vector3 dimensions = TGen::Vector3::Parse(properties.getNode("mass").getProperty("dimensions", "1.0 1.0 1.0"));
			
			dMassSetBox(&mass, totalMass, dimensions.x, dimensions.y, dimensions.z);
		}
		else if (massType == "sphere") {
			scalar totalMass = TGen::lexical_cast<scalar>(properties.getNode("mass").getProperty("total", "1.0"));
			scalar radius = TGen::lexical_cast<scalar>(properties.getNode("mass").getProperty("radius", "1.0"));
			
			dMassSetSphere(&mass, totalMass, radius);
		}
		else {
			throw TGen::RuntimeException("PhysicsSubsystem::createBody", "invalid mass type '" + massType + "'");
		}
		
		dBodySetMass(newBodyId, &mass);
	}
	catch (...) {
		// No mass defined
	}
	
	TGen::Engine::BodyComponent * newBody = new TGen::Engine::BodyComponent("physBody", newBodyId);
	newBody->setPosition(position);
	bodies.push_back(newBody);
	
	return newBody;
}

TGen::Engine::JointComponent * TGen::Engine::Physics::Subsystem::createJoint(const TGen::PropertyTree & properties) {
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

TGen::Engine::GeomComponent * TGen::Engine::Physics::Subsystem::createGeom(const TGen::PropertyTree & properties) {
	if (properties.getNumAttributes() == 0)
		throw TGen::RuntimeException("PhysicsSubsystem::createJoint", "no attributes, plz give some");

	std::string geomType = properties.getAttribute(0);

	TGen::Engine::GeomComponent * newComponent = NULL;
	
	if (geomType == "plane") {
		TGen::Vector3 normal = TGen::Vector3::Parse(properties.getProperty("orientation", "0 1 0"));
		scalar distance = TGen::lexical_cast<scalar>(properties.getProperty("distance", "0"));
		
		newComponent = new TGen::Engine::PlaneGeomComponent("physGeom", TGen::Plane3(normal, distance), mainSpace);
	}
	else if (geomType == "sphere") {
		scalar radius = TGen::lexical_cast<scalar>(properties.getProperty("radius", "1.0"));
		
		newComponent = new TGen::Engine::SphereGeomComponent("physGeom", radius, mainSpace);
	}
	else if (geomType == "box") {
		TGen::Vector3 dimensions = TGen::Vector3::Parse(properties.getProperty("dimensions", "1 1 1"));
		
		newComponent = new TGen::Engine::BoxGeomComponent("physGeom", dimensions, mainSpace);
	}
	else {
		throw TGen::RuntimeException("PhysicsSubsystem::createGeom", "invalid geom type '" + geomType + "'!");
	}
	
	newComponent->setFriction(TGen::lexical_cast<float>(properties.getProperty("friction", "1.0")));
	
	return newComponent;
}

TGen::Engine::PhysicsPropertiesComponent * TGen::Engine::Physics::Subsystem::createProps(const TGen::PropertyTree & properties) {
	//TGen::Engine::PhysicsPropertiesComponent * newComponent = new TGen::Engine::PhysicsPropertiesComponent;
	//newComponent->friction = TGen::lexical_cast<float>(properties.getProperty("friction", "1.0"));
	
	return NULL;// newComponent;
}

void TGen::Engine::Physics::Subsystem::link() {
	
}

void TGen::Engine::Physics::Subsystem::update(scalar dt) {
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

void TGen::Engine::Physics::Subsystem::setGravity(const TGen::Vector3 & gravity) {
	dWorldSetGravity(worldId, gravity.x, gravity.y, gravity.z);
}

void TGen::Engine::Physics::Subsystem::nearCallback(void * data, dGeomID o1, dGeomID o2) {
	if (dGeomIsSpace(o1) || dGeomIsSpace(o2)) {
		dSpaceCollide2(o1, o2, data, &nearCallback);
		
		//if (dGeomIsSpace(o1))
		//	dSpaceCollide(o1, data, &nearCallback);
		//if (dGeomIsSpace(o2))
		//	dSpaceCollide(o2, data, &nearCallback);
	}
	else {
		const int MAX_CONTACTS = 20;
		dContact contacts[MAX_CONTACTS];
		int numContacts = dCollide(o1, o2, MAX_CONTACTS, &contacts[0].geom, sizeof(dContact));
		
		dBodyID body1 = dGeomGetBody(o1);
		dBodyID body2 = dGeomGetBody(o2);
		
		
		// +bounce, soft
		
		TGen::Engine::GeomComponent * geom1 = static_cast<TGen::Engine::GeomComponent *>(dGeomGetData(o1));
		TGen::Engine::GeomComponent * geom2 = static_cast<TGen::Engine::GeomComponent *>(dGeomGetData(o2));
		
		
		for (int i = 0; i < numContacts; ++i) {
			contacts[i].surface.mode = 0;
			contacts[i].surface.mu = geom1->getFriction() * geom2->getFriction();
			dJointID contactJoint = dJointCreateContact(worldId, contactGroup, &contacts[i]);
			dJointAttach(contactJoint, body1, body2);
		}
	}
	
}

