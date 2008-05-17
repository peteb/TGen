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
#include "physics/body.h"
#include "physics/joint.h"
#include "physics/planegeom.h"
#include "physics/spheregeom.h"
#include "physics/boxgeom.h"
#include "physics/geom.h"
#include "physics/bipedalgeom.h"
#include "physics/id4cmgeom.h"
#include "physics/id4cmloader.h"
#include <ode/ode.h>
#include <tgen_math.h>

//dWorldID TGen::Engine::Physics::Subsystem::worldId = 0;
dJointGroupID TGen::Engine::Physics::Subsystem::contactGroup = 0;

TGen::Engine::Physics::Subsystem::Subsystem(TGen::Engine::StandardLogs & logs, TGen::Engine::Filesystem & filesystem) 
	: logs(logs)
	, updateInterval(0.01)
	, worldId(0)
	, filesystem(filesystem)
{
	logs.info["phys+"] << "*** INITIALIZING PHYSICS ***" << TGen::endl;
	
	worldId = dWorldCreate();
	mainSpace = dSimpleSpaceCreate(0);
	contactGroup = dJointGroupCreate(0);
	dWorldSetLinearDamping(worldId, 0.07);
	setGravity(TGen::Vector3(0.0f, -50.0f, 0.0f));
}

TGen::Engine::Physics::Subsystem::~Subsystem() {
	logs.info["phys-"] << "*** SHUTTING DOWN PHYSICS ***" << TGen::endl;
	
	dCloseODE();
}

TGen::Engine::Component * TGen::Engine::Physics::Subsystem::createComponent(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties) {
	if (properties.getName() == "physBody")
		return createBody(name, properties);
	else if (properties.getName() == "physJoint")
		return createJoint(name, properties);
	else if (properties.getName() == "physGeom")
		return createGeom(name, properties);
	
	throw TGen::RuntimeException("PhysicsSubsystem::createComponent", "invalid component type '" + properties.getName() + "'");
}

TGen::Engine::Physics::Body * TGen::Engine::Physics::Subsystem::createBody(const std::string & name, const TGen::PropertyTree & properties) {
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
	
	//dBodySetLinearDamping(newBodyId, 0.07);
	
	TGen::Engine::Physics::Body * newBody = new TGen::Engine::Physics::Body(name, newBodyId, worldId, mainSpace);
	
	dBodySetData(newBodyId, reinterpret_cast<void *>(newBody));
	newBody->setPosition(position);
	newBody->setTurnHeadwise(TGen::lexical_cast<bool>(properties.getProperty("turnHead", "false")));
	newBody->setMaxAngularSpeed(TGen::lexical_cast<scalar>(properties.getProperty("maxAngularSpeed", "-1.0")));
	newBody->setNodeComponent(properties.getProperty("link", "sceneNode"));
	newBody->setKillTorque(TGen::lexical_cast<bool>(properties.getProperty("killTorque", "false")));
	newBody->setLinearDamping(TGen::lexical_cast<scalar>(properties.getProperty("linearDamping", "0.0")));
	newBody->setFakeGravity(TGen::lexical_cast<scalar>(properties.getProperty("fakeGrav", "-2.0")));
	
	bodies.push_back(newBody);
	
	return newBody;
}

TGen::Engine::Physics::Geom * TGen::Engine::Physics::Subsystem::createGeom(const std::string & name, const TGen::PropertyTree & properties) {
	//if (properties.getNumAttributes() == 0)
	//		throw TGen::RuntimeException("Physics::Subsystem::createGeom", "no attributes, plz give some");
	
	std::auto_ptr<TGen::Engine::Physics::Geom> newGeom;
	std::string geomType = properties.getProperty("type", "none");
	
	if (geomType == "plane") {
		TGen::Vector3 normal = TGen::Vector3::Parse(properties.getProperty("orientation", "0 1 0"));
		scalar distance = TGen::lexical_cast<scalar>(properties.getProperty("distance", "0"));
		
		newGeom.reset(new TGen::Engine::Physics::PlaneGeom(name, TGen::Plane3(normal, distance), mainSpace));
	}
	else if (geomType == "sphere") {
		scalar radius = TGen::lexical_cast<scalar>(properties.getProperty("radius", "1.0"));
		
		newGeom.reset(new TGen::Engine::Physics::SphereGeom("physGeom", radius, mainSpace));
	}
	else if (geomType == "box") {
		TGen::Vector3 dimensions = TGen::Vector3::Parse(properties.getProperty("dimensions", "1 1 1"));
		
		newGeom.reset(new TGen::Engine::Physics::BoxGeom("physGeom", dimensions, mainSpace));
	}
	else if (geomType == "bipedal") {
		newGeom.reset(new TGen::Engine::Physics::BipedalGeom(name, mainSpace, 
																			  TGen::lexical_cast<scalar>(properties.getProperty("capRadius", "1.0")), 
																			  TGen::lexical_cast<scalar>(properties.getProperty("length", "1.0"))));
	}
	else if (geomType == "id4cm") {
		TGen::Engine::Physics::Id4CMLoader loader(filesystem);
		
		newGeom.reset(loader.createGeom(properties.getProperty("model", "")));
	}
	
	if (!newGeom.get())
		throw TGen::RuntimeException("Physics::Subsystem::createGeom", "invalid geom type '" + geomType + "'!");
	
	newGeom->setFriction(TGen::lexical_cast<float>(properties.getProperty("friction", "1.0")));
	newGeom->setBodyComponent(properties.getProperty("link", "physBody"));
	newGeom->setAffectsOthers(TGen::lexical_cast<bool>(properties.getProperty("affectsOthers", "true")));
	geoms.push_back(newGeom.get());
	
	return newGeom.release();
}

TGen::Engine::Physics::Joint * TGen::Engine::Physics::Subsystem::createJoint(const std::string & name, const TGen::PropertyTree & properties) {
	if (properties.getNumAttributes() == 0)
		throw TGen::RuntimeException("PhysicsSubsystem::createJoint", "no attributes, plz give some");
	
	std::string jointType = properties.getAttribute(0);
	
	dJointID newJointId;
	
	if (jointType == "ball")
		newJointId = dJointCreateBall(worldId, 0);
	else
		throw TGen::RuntimeException("PhysicsSubsystem::createJoint", "joint type '" + jointType + "' invalid");
	
	TGen::Engine::Physics::Joint * newComponent = new TGen::Engine::Physics::Joint(name, newJointId, properties.getProperty("attach", ""));
	newComponent->setAnchor(TGen::Vector3::Parse(properties.getProperty("anchor", "0 0 0")));
	
	return newComponent;
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
		
		for (int i = 0; i < geoms.size(); ++i)
			geoms[i]->preStep();
				
		dSpaceCollide(mainSpace, 0, &nearCallback);

		dWorldStep(worldId, updateInterval); // tweak

		dJointGroupEmpty(contactGroup);
		
		
		for (int i = 0; i < bodies.size(); ++i)
			bodies[i]->postStep();
		
		for (int i = 0; i < geoms.size(); ++i)
			geoms[i]->postStep();
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
		const int MAX_CONTACTS = 50;
		dContact contacts[MAX_CONTACTS];
		int numContacts = dCollide(o1, o2, MAX_CONTACTS, &contacts[0].geom, sizeof(dContact));
		
		dBodyID body1 = dGeomGetBody(o1);
		dBodyID body2 = dGeomGetBody(o2);
		
		
		// +bounce, soft
		
		TGen::Engine::Physics::Geom * geom1 = static_cast<TGen::Engine::Physics::Geom *>(dGeomGetData(o1));
		TGen::Engine::Physics::Geom * geom2 = static_cast<TGen::Engine::Physics::Geom *>(dGeomGetData(o2));
		TGen::Engine::Physics::Body * bodyObject1 = NULL, * bodyObject2 = NULL;
		dWorldID worldId = 0;
		
		if (body1) {
			bodyObject1 = reinterpret_cast<TGen::Engine::Physics::Body *>(dBodyGetData(body1));
			
			if (bodyObject1 && bodyObject1->getWorldId())
				worldId = bodyObject1->getWorldId();
		}
		
		if (body2) {
			bodyObject2 = reinterpret_cast<TGen::Engine::Physics::Body *>(dBodyGetData(body2));
			
			if (bodyObject2 && bodyObject2->getWorldId())
				worldId = bodyObject2->getWorldId();
		}
		
		if (!worldId) {
			//std::cout << "bad world" << std::endl;
			return;
		}
		
		//if (bodyObject)
		//	std::cout << "HEY" << std::endl;
		
		//if (geom1 && !geom1->getAffectsOthers())
		//	body2 = 0;
		
		//if (geom2 && !geom2->getAffectsOthers())
		//	body1 = 0;
		
		
		for (int i = 0; i < numContacts; ++i) {
			TGen::Vector3 contactNormal(contacts[i].geom.normal[0], contacts[i].geom.normal[1], contacts[i].geom.normal[2]);
			scalar dp = TGen::Vector3::DotProduct(contactNormal, TGen::Vector3(0.0f, -1.0f, 0.0f));
			
			TGen::Degree slope(TGen::Radian(acos(dp)));
				
			if (slope.angle >= -35.0f && slope.angle <= 35.0f) {
				if (bodyObject1)
					bodyObject1->setOnFloor(true);
				
				if (bodyObject2)
					bodyObject2->setOnFloor(true);
			}			

			scalar friction = 1.0f;
			if (geom1)
				friction *= geom1->getFriction();
			
			if (geom2)
				friction *= geom2->getFriction();
			
			contacts[i].surface.mode = 0;
			contacts[i].surface.mu = friction;
			
			if ((!geom1 || (geom1 && geom1->onCollision(geom2, o1, contacts[i]))) && (!geom2 || (geom2 && geom2->onCollision(geom1, o2, contacts[i])))) {

				dJointID contactJoint = dJointCreateContact(worldId, contactGroup, &contacts[i]);
				dJointAttach(contactJoint, body1, body2);

			}
		}
	}
	
}

