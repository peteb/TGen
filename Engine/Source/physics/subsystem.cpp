/*
 *  physicssubsystem.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 2/29/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "physics/subsystem.h"
#include "physics/body.h"
#include "physics/joint.h"
#include "physics/planegeom.h"
#include "physics/spheregeom.h"
#include "physics/boxgeom.h"
#include "physics/geom.h"
#include "physics/bipedalgeom.h"
#include "physics/meshgeom.h"
#include "physics/id4cmgeom.h"
#include "physics/id4cmloader.h"
#include "physics/geomrecipe.h"
#include "physics/bodyrecipe.h"

#include "generateline.h"
#include "log.h"
#include "transformerfactory.h"

#include <ode/ode.h>
#include <tgen_math.h>
#include <tgen_renderer.h>

using TGen::uint;

//dWorldID TGen::Engine::Physics::Subsystem::worldId = 0;
dJointGroupID TGen::Engine::Physics::Subsystem::contactGroup = 0;

TGen::Engine::Physics::Subsystem::Subsystem(TGen::Engine::StandardLogs & logs, TGen::Engine::Filesystem & filesystem) 
	: logs(logs)
	, updateInterval(1.0/100.0)
	, worldId(0)
	, filesystem(filesystem)
	, lastUpdate(0.0f)
{
	logs.info["phys+"] << "*** INITIALIZING PHYSICS ***" << TGen::endl;
	
	worldId = dWorldCreate();
	mainSpace = dSimpleSpaceCreate(0);
	contactGroup = dJointGroupCreate(0);
	dWorldSetLinearDamping(worldId, 0.07);
	setGravity(TGen::Vector3(0.0f, -40.0f, 0.0f));
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


TGen::Engine::ComponentRecipe * TGen::Engine::Physics::Subsystem::createComponentRecipe(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties) {
	TGen::Engine::ComponentRecipe * ret = NULL;
	
	if (properties.getName() == "physGeom") {
		std::string type = properties.getProperty("type", "unknown");

		TGen::Engine::Physics::GeomRecipe * newRecipe = NULL; 
		
		if (type == "sphere") {
			newRecipe = new TGen::Engine::Physics::GeomRecipe(TGen::Engine::Physics::SphereGeomType, name, mainSpace, *this);
			newRecipe->setScalarValue1(TGen::lexical_cast<scalar>(properties.getProperty("radius", "1.0")));
		}
		else if (type == "ccylinder") {
			newRecipe = new TGen::Engine::Physics::GeomRecipe(TGen::Engine::Physics::CappedCylinderGeomType, name, mainSpace, *this);
			newRecipe->setScalarValue1(TGen::lexical_cast<scalar>(properties.getProperty("radius", "1.0")));
			newRecipe->setScalarValue2(TGen::lexical_cast<scalar>(properties.getProperty("length", "1.0")));
		}
		else if (type == "box") {
			newRecipe = new TGen::Engine::Physics::GeomRecipe(TGen::Engine::Physics::BoxGeomType, name, mainSpace, *this);
			TGen::Vector3 dimensions = TGen::Vector3::Parse(properties.getProperty("dimensions", "1 1 1"));
			
			newRecipe->setScalarValue1(dimensions.x);
			newRecipe->setScalarValue2(dimensions.y);
			newRecipe->setScalarValue3(dimensions.z);
		}
		else if (type == "ray") {
			newRecipe = new TGen::Engine::Physics::GeomRecipe(TGen::Engine::Physics::RayGeomType, name, mainSpace, *this);
			newRecipe->setScalarValue1(TGen::lexical_cast<scalar>(properties.getProperty("length", "1.0")));
			
		}
		else {
			throw TGen::RuntimeException("Physics::Subsystem::createComponentRecipe", "invalid geom type: " + type);
		}
		
		newRecipe->setLink(properties.getProperty("link", "sceneNode"));
		newRecipe->setFriction(TGen::lexical_cast<scalar>(properties.getProperty("friction", "10")));
		uint collideWith = ~getCategoryBits(properties.getProperty("noCollide", ""));
		
		newRecipe->setCategory(getCategoryBits(properties.getProperty("category", "default")));
		newRecipe->setCollidesWith(collideWith);
		
		ret = newRecipe;
	}
	else if (properties.getName() == "physBody") {
		TGen::Engine::Physics::BodyRecipe * newRecipe = new TGen::Engine::Physics::BodyRecipe(name, mainSpace, getMass(properties), *this);
		
		newRecipe->setLink(properties.getProperty("link", "sceneNode"));
		ret = newRecipe;
	}
	else {
		throw TGen::RuntimeException("Physics::Subsystem::createComponentRecipe", "invalid component type: " + properties.getName());
	}

	
	return ret;
}


TGen::Engine::Physics::Body * TGen::Engine::Physics::Subsystem::createBody(const std::string & name, const TGen::PropertyTree & properties) {
	TGen::Vector3 position = TGen::Vector3::Parse(properties.getProperty("position", "0 0 0"));
	bool applyGravity = TGen::lexical_cast<bool>(properties.getProperty("gravity", "true"));
	
	dBodyID newBodyId = dBodyCreate(worldId);
	dBodySetGravityMode(newBodyId, applyGravity);
	
	try {
		dMass mass = getMass(properties);
		
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
	newBody->setLink(properties.getProperty("link", "sceneNode"));
	newBody->setKillTorque(TGen::lexical_cast<bool>(properties.getProperty("killTorque", "false")));
	newBody->setLinearDamping(TGen::lexical_cast<scalar>(properties.getProperty("linearDamping", "0.0")));
	newBody->setFakeGravity(TGen::lexical_cast<scalar>(properties.getProperty("fakeGrav", "-2.0")));
	
	bodies.push_back(newBody);
	
	return newBody;
}


dMass TGen::Engine::Physics::Subsystem::getMass(const TGen::PropertyTree & properties) {
	dMass ret;
	std::string massType = properties.getProperty("massType", "unknown");

	if (massType == "box") {
		scalar totalMass = TGen::lexical_cast<scalar>(properties.getProperty("massTotal", "1.0"));
		TGen::Vector3 dimensions = TGen::Vector3::Parse(properties.getProperty("massDimensions", "1.0 1.0 1.0"));
		
		dMassSetBox(&ret, totalMass, dimensions.x, dimensions.y, dimensions.z);
	}
	else if (massType == "sphere") {
		scalar totalMass = TGen::lexical_cast<scalar>(properties.getProperty("massTotal", "1.0"));
		scalar radius = TGen::lexical_cast<scalar>(properties.getProperty("massRadius", "1.0"));
		
		dMassSetSphere(&ret, totalMass, radius);
	}
	else {
		throw TGen::RuntimeException("Physics::Subsystem::getMass", "invalid mass type '" + massType + "'");
	}
	
	return ret;
}


void TGen::Engine::Physics::Subsystem::addGeom(TGen::Engine::Physics::Geom * geom) {
	geoms.push_back(geom);
}


void TGen::Engine::Physics::Subsystem::addBody(TGen::Engine::Physics::Body * body) {
	bodies.push_back(body);
}


TGen::Engine::Physics::Geom * TGen::Engine::Physics::Subsystem::createGeom(const std::string & name, const TGen::PropertyTree & properties) {
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
	else if (geomType == "mesh") {
		TGen::PropertyTree props(properties);
		
		if (!props.hasNode("normals"))
			props.addNode(TGen::PropertyTree("normals"));
		
		newGeom.reset(new TGen::Engine::Physics::MeshGeom(name, mainSpace, props.getNode("vertices"), 
																		  props.getNode("indices"), 
																		  props.getNode("normals")));
	}
	else if (geomType == "id4cm") {
		TGen::Engine::Physics::Id4CMLoader loader(filesystem);
		TGen::Engine::GenerateLine line("gen:" + properties.getProperty("model", ""));
		TGen::Engine::TransformerFactory transFactory;
		
		TGen::VertexTransformList transformers;
		transformers.addTransformer(transFactory.createTransformers(line));
		
		newGeom.reset(loader.createGeom(name, line.getName(), transformers, mainSpace));
	}
	
	if (!newGeom.get())
		throw TGen::RuntimeException("Physics::Subsystem::createGeom", "invalid geom type '" + geomType + "'!");
	
	newGeom->setFriction(TGen::lexical_cast<float>(properties.getProperty("friction", "1.0")));
	newGeom->setLink(properties.getProperty("link", "physBody"));
	newGeom->setAffectsOthers(TGen::lexical_cast<bool>(properties.getProperty("affectsOthers", "true")));
	
	
	uint collideWith = ~getCategoryBits(properties.getProperty("noCollide", ""));
	
	newGeom->setCategory(getCategoryBits(properties.getProperty("category", "default")));
	newGeom->setCollidesWith(collideWith);
	
	
	
	geoms.push_back(newGeom.get());
	
	return newGeom.release();
}


uint TGen::Engine::Physics::Subsystem::getCategoryBits(const std::string & name) {
	static TGen::SymbolTable symbols;
	
	if (name.empty())
		return 0;
	
	uint ret = 0;
	uint pos = 0;
	
	while (1) {
		int nextPos = name.find(" ", pos);
		std::string fixedName = name;

		if (nextPos == std::string::npos)
			fixedName = name.substr(pos);
		else
			fixedName = name.substr(pos, nextPos - pos);
		
		int symbolNumber = symbols[name];
		
		ret |= 1 << symbolNumber;
		
		if (nextPos == std::string::npos)
			break;
		
		pos = nextPos + 1;
	}
	
	return ret;
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
	
	TGen::Engine::Physics::Joint * newComponent = new TGen::Engine::Physics::Joint(name, newJointId);
	newComponent->setLink1(properties.getProperty("attach1", ""));
	newComponent->setLink1(properties.getProperty("attach2", ""));
	newComponent->setAnchor(TGen::Vector3::Parse(properties.getProperty("anchor", "0 0 0")));
	
	return newComponent;
}


void TGen::Engine::Physics::Subsystem::link() {
	
}


void TGen::Engine::Physics::Subsystem::update(scalar dt) {
	lastUpdate += dt;
	
	if (lastUpdate < updateInterval)
		return;


	for (int i = 0; i < bodies.size(); ++i)
		bodies[i]->preStep();
	
	for (int i = 0; i < geoms.size(); ++i)
		geoms[i]->preStep();
		
	int updates = 0;
	
	while (lastUpdate >= updateInterval) {				
		if (updates++ > 10) {
			lastUpdate = 0.0;
			break;
		}
		
		dSpaceCollide(mainSpace, 0, &nearCallback);
		dWorldStep(worldId, updateInterval); // tweak

		dJointGroupEmpty(contactGroup);
		
		lastUpdate -= updateInterval;		
	}

	// TODO: spridning på skott
	// TODO: hur ska man göra så elden visas? från vapen
	//  ska man spawna en ny nod eller sätta visibility på en som redan finns?
	//  kolla hur md3 gör.. men det blir nog en event isf, så vänta
	
	for (int i = 0; i < bodies.size(); ++i)
		bodies[i]->postStep();
	
	for (int i = 0; i < geoms.size(); ++i)
		geoms[i]->postStep();	
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
		
		if (geom1 && !geom1->getAffectsOthers())
			body2 = 0;
		
		if (geom2 && !geom2->getAffectsOthers())
			body1 = 0;
		
		for (int i = 0; i < numContacts; ++i) {
			TGen::Vector3 contactNormal(contacts[i].geom.normal[0], contacts[i].geom.normal[1], contacts[i].geom.normal[2]);
			scalar dp = TGen::Vector3::DotProduct(contactNormal, TGen::Vector3(0.0f, -1.0f, 0.0f));
			
			TGen::Degree slope(TGen::Radian(acos(dp)));
				
			bool onGround = false;
			
			if (geom1->getCategory() == 2 || geom2->getCategory() == 2) {
				//std::cout << "geom1: " << std::hex << geom1->getCategory() << " geom2: " << geom2->getCategory() << " normal: " << std::string(contactNormal) << std::endl;
				
				if (bodyObject1) {
					scalar dp = TGen::Vector3::DotProduct(contactNormal, TGen::Vector3(0.0f, 1.0f, 0.0f));
					
					if (fabs(dp) <= 1.2f && fabs(dp) >= 0.8) {
						bodyObject1->setGroundNormal(contactNormal);
						bodyObject1->setOnFloor(true);

						//	std::cout << "body1: " << dp << std::endl;
					}
					else {
						//bodyObject1->setGroundNormal(TGen::Vector3(0.0f, -1.0f, 0.0f));
					}
				}
				if (bodyObject2) {
					scalar dp = TGen::Vector3::DotProduct(-contactNormal, TGen::Vector3(0.0f, 1.0f, 0.0f));
					
					if (fabs(dp) <= 1.2f && fabs(dp) >= 0.8) {
						bodyObject2->setGroundNormal(-contactNormal);
						bodyObject2->setOnFloor(true);
						//std::cout << "body2: " << dp << std::endl;
					}
					else {
						//bodyObject2->setGroundNormal(TGen::Vector3(0.0f, -1.0f, 0.0f));
					}
				}
			}
			
			//if (slope.angle >= -80.0f && slope.angle <= 80.0f) {

				onGround = true;
			//}			
			
			scalar friction = 1.0f;
			scalar dp2 = TGen::Vector3::DotProduct(contactNormal, TGen::Vector3(0.0f, 1.0f, 0.0f));

			if (geom1) {
				friction *= geom1->getFriction();
			}
			
			if (geom2) {
				friction *= geom2->getFriction();
			}
			
			if (!onGround)
				friction = 0.0f;		// TODO: det här är fulhack...
			else
				friction *= (1.0 + fabs(dp2));
			
			contacts[i].surface.mode = 0;
			contacts[i].surface.mu = friction;
			
			TGen::Vector3 force1, force2;
			
			if (bodyObject1)
				force1 = bodyObject1->getLinearVelocity();
			
			if (bodyObject2)
				force2 = bodyObject2->getLinearVelocity();
			
			scalar totalForce = fabs(TGen::Vector3::DotProduct(force1 - force2, contactNormal));

			// each force should be multiplied by mass
			
			if (geom1->getCategory() == 2 || geom2->getCategory() == 2) {
				scalar dir = 1.0f;
				
				/*if (geom1->getCategory() == 2)
					dir *= TGen::Vector3::DotProduct(force1.getNormalized(), contactNormal);
				
				if (geom2->getCategory() == 2)
					dir *= TGen::Vector3::DotProduct(force2.getNormalized(), contactNormal);
				*/
				//std::cout << dir << " DIR" << std::endl;
				
				totalForce -= 10.0;
				totalForce = std::max(totalForce, 0.0f);
				//totalForce *= dir;
				
				if (totalForce > 0.1)
					std::cout << "HURT: " << totalForce << std::endl;
			}
			
			if ((!geom1 || (geom1 && geom1->onCollision(geom2, o1, contacts[i]))) && (!geom2 || (geom2 && geom2->onCollision(geom1, o2, contacts[i])))) {

				dJointID contactJoint = dJointCreateContact(worldId, contactGroup, &contacts[i]);
				dJointAttach(contactJoint, body1, body2);

			}
		}
	}
	
}

// FIXA MATERIAL FÖR SENASTE GREJORNA

dWorldID TGen::Engine::Physics::Subsystem::getWorldId() {
	return worldId;
}



