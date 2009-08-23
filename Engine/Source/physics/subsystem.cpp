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
#include "physics/geom.h"

#include "log.h"

using TGen::uint;

int TGen::Engine::Physics::Subsystem::numSubsystems = 0;

//dWorldID TGen::Engine::Physics::Subsystem::worldId = 0;
dJointGroupID TGen::Engine::Physics::Subsystem::contactGroup = 0;
int TGen::Engine::Physics::Subsystem::reportedCollisions = 0;
float TGen::Engine::Physics::Subsystem::lastSecond = 0.0f;
std::vector<dContact> TGen::Engine::Physics::Subsystem::collisionEvents;

TGen::Engine::Physics::Subsystem::Subsystem(TGen::Engine::StandardLogs & logs, TGen::Engine::Filesystem & filesystem) 
	: logs(logs)
	, updateInterval(1.0/100.0)	// 60 är rekommenderat enl sun
	, filesystem(filesystem)
	, lastUpdate(0.0f)
	, componentFactory(*this, filesystem)
{
	logs.info["phys+"] << "*** INITIALIZING PHYSICS ***" << TGen::endl;
	
	dInitODE();

	worldId = dWorldCreate();

	std::cout << "ERP: " << dWorldGetERP(worldId) << std::endl;
	std::cout << "CFM: " << dWorldGetCFM(worldId) << std::endl;

	dWorldSetERP(worldId, 0.3f);
	dWorldSetCFM(worldId, 0.00001f);
	
	dVector3 center = {0.0f, 0.0f, 0.0f};
	dVector3 extents = {500.0f, 500.0f, 500.0f};
	mainSpace = dQuadTreeSpaceCreate(0, center, extents, 7); //dHashSpaceCreate(0); //dSimpleSpaceCreate(0);
	contactGroup = dJointGroupCreate(0);
	dWorldSetLinearDamping(worldId, 0.07);
	
	dWorldSetAutoDisableFlag(worldId, true);
	
	setGravity(TGen::Vector3(0.0f, -30.0f, 0.0f));
	
	numSubsystems++;
}


TGen::Engine::Physics::Subsystem::~Subsystem() {
	logs.info["phys-"] << "*** SHUTTING DOWN PHYSICS ***" << TGen::endl;

	dSpaceDestroy(mainSpace);
	dWorldDestroy(worldId);
	
	if (numSubsystems-- <= 0)
		dCloseODE();
	
}


TGen::Engine::Component * TGen::Engine::Physics::Subsystem::createComponent(const std::string & name, TGen::Engine::Entity & entity, const TGen::PropertyTree & properties) {
	std::string type = properties.getName();
	TGen::Engine::Component * ret = NULL;
	
	if (type == "physBody") {
		Body * newBody = componentFactory.createBody(name, properties, worldId, mainSpace, entity);
		addBody(newBody);
		
		ret = newBody;	
		
		
		//TGen::Engine::ComponentRecipe * recipe = componentFactory.createComponentRecipe(name, entityName, properties);
		
	}
	else if (type == "physGeom") {
		Geom * newGeom = componentFactory.createGeom(name, properties, mainSpace, entity);
		addGeom(newGeom);
		
		ret = newGeom;
	}
	else if (type == "physJoint") {
		ret = componentFactory.createJoint(name, properties, worldId);
	}
	else {
		throw TGen::RuntimeException("Physics::Subsystem::createComponent", "invalid component type '" + properties.getName() + "'");
	}
	
	return ret;
}


TGen::Engine::ComponentRecipe * TGen::Engine::Physics::Subsystem::createComponentRecipe(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties) {
	return componentFactory.createComponentRecipe(name, entityName, properties);
}


void TGen::Engine::Physics::Subsystem::addGeom(TGen::Engine::Physics::Geom * geom) {
	geoms.push_back(geom);
	
	addSubGeoms(geom);
}

void TGen::Engine::Physics::Subsystem::addSubGeoms(TGen::Engine::Physics::Geom * geom) {
	for (int i = 0; i < geom->getNumChildren(); ++i) {
		Geom * child = dynamic_cast<Geom *>(geom->getChild(i));
		
		if (child) {
			geoms.push_back(child);
			addSubGeoms(child);
		}
	}
}

void TGen::Engine::Physics::Subsystem::addBody(TGen::Engine::Physics::Body * body) {
	bodies.push_back(body);
}


void TGen::Engine::Physics::Subsystem::link() {
	for (int i = 0; i < bodies.size(); ++i)
		bodies[i]->preStep();
	
	for (int i = 0; i < geoms.size(); ++i)
		geoms[i]->preStep();
	
	for (int i = 0; i < 1000; ++i)		// should make the physics settle down
		dWorldStep(worldId, 0.0001);
}


void TGen::Engine::Physics::Subsystem::update(scalar dt) {
	lastUpdate += dt;
	
	static int steps = 0;
	static float timeStepped = 0.0f;
	
	lastSecond += dt;
	
	if (lastSecond >= 1.0f) {
		std::cout << "geoms: " << dSpaceGetNumGeoms(mainSpace) << std::endl;
		std::cout << "collisions last sec: " << reportedCollisions << std::endl;
		std::cout << "steps: " << steps << " time: " << timeStepped << std::endl;
		
		lastSecond -= floor(lastSecond);
		reportedCollisions = 0;
		timeStepped = 0.0f;
		steps = 0;
	}
	
	if (lastUpdate < updateInterval)
		return;

	for (int i = 0; i < bodies.size(); ++i)
		bodies[i]->preStep();
	
	for (int i = 0; i < geoms.size(); ++i) {
		geoms[i]->updateClock(dt);
		geoms[i]->preStep();
	}
		
	int updates = 0;
	
	while (lastUpdate >= updateInterval) {				
		if (updates++ > 20) {
			std::cout << "Physics hit step ceiling" << std::endl;
			lastUpdate = 0.0;
			break;
		}
		
		dSpaceCollide(mainSpace, 0, &nearCallback);
		
		steps++;
		timeStepped += updateInterval;
		
		// force is not per second but per step????
		
		dWorldQuickStep(worldId, updateInterval); // tweak
		
		dJointGroupEmpty(contactGroup);

		lastUpdate -= updateInterval;		
	}

	// TODO: hur ska man göra så elden visas? från vapen
	//  ska man spawna en ny nod eller sätta visibility på en som redan finns?
	//  kolla hur md3 gör.. men det blir nog en event isf, så vänta
	
	
	for (int i = 0; i < geoms.size(); ++i)
		geoms[i]->postStep();	

	for (int i = 0; i < bodies.size(); ++i)
		bodies[i]->postStep();

	triggerCollisionEvents();		
	collisionEvents.clear();

}


void TGen::Engine::Physics::Subsystem::setGravity(const TGen::Vector3 & gravity) {
	dWorldSetGravity(worldId, gravity.x, gravity.y, gravity.z);
}


TGen::Vector3 TGen::Engine::Physics::Subsystem::getGravity() const {
	dReal gravity[3];
	dWorldGetGravity(worldId, gravity);
	
	return TGen::Vector3(gravity[0], gravity[1], gravity[2]);
}



void TGen::Engine::Physics::Subsystem::nearCallback(void * data, dGeomID o1, dGeomID o2) {
	// the most scariest function in the whole engine
	reportedCollisions++;
	
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
			bool body1OnGround = false, body2OnGround = false;
			
			if (geom1->getAffectsOthers() && geom2->getAffectsOthers()) {
			//if (geom1->getCategory() == 2 || geom2->getCategory() == 2) {
				if (bodyObject1) {
					scalar dp = TGen::Vector3::DotProduct(contactNormal, TGen::Vector3(0.0f, 1.0f, 0.0f));
					scalar groundDef = bodyObject1->getGroundDefinition();
					
					if (fabs(dp) <= 1.0f + groundDef && fabs(dp) >= 1.0f - groundDef) {
						bodyObject1->setGroundNormal(contactNormal);
						bodyObject1->setOnFloor(true);
						body1OnGround = true;
						//	std::cout << "body1: " << dp << std::endl;
					}
					else {
						//bodyObject1->setGroundNormal(TGen::Vector3(0.0f, -1.0f, 0.0f));
					}
				}
				if (bodyObject2) {
					scalar dp = TGen::Vector3::DotProduct(-contactNormal, TGen::Vector3(0.0f, 1.0f, 0.0f));
					scalar groundDef = bodyObject2->getGroundDefinition();
					
					if (fabs(dp) <= 1.0f + groundDef && fabs(dp) >= 1.0f - groundDef) {
						bodyObject2->setGroundNormal(-contactNormal);
						bodyObject2->setOnFloor(true);
						body2OnGround = true;

						//std::cout << "body2: " << dp << std::endl;
					}
					else {
						//bodyObject2->setGroundNormal(TGen::Vector3(0.0f, -1.0f, 0.0f));
					}
				}
			}
		//}
			
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
				force1 = bodyObject1->getLinearVelocity() * bodyObject1->getMass();
			
			if (bodyObject2)
				force2 = bodyObject2->getLinearVelocity() * bodyObject2->getMass();
			
			scalar totalForce = fabs(TGen::Vector3::DotProduct(force1 - force2, contactNormal));

			
			//std::cout << geom1->getCategory() << " (" << std::string(force1) << ") " << geom2->getCategory() << " (" << std::string(force2) << ")" << std::endl;
			
			// each force should be multiplied by mass
			
			//if (geom1->getCategory() == 2 || geom2->getCategory() == 2) {
				scalar dir = 1.0f;
				
				/*if (totalForce >= 10.0) {
					std::cout << "CAT1: " << (geom1->getCategory() == 2) << " FORCE1: " << std::string(force1) << " F2: " << std::string(force2) << std::endl;
				
					std::cout << "M1: " << bodyObject1->getMass() << " M2: " << bodyObject2->getMass() << std::endl;
					std::cout << "S1: " << std::fixed <<  std::string(bodyObject1->getLinearVelocity()) << " S2: " << std::string(bodyObject2->getLinearVelocity()) << std::endl;
					
					TGen::Vector3 diff = bodyObject2->getLinearVelocity() * bodyObject2->getMass() - bodyObject1->getLinearVelocity() * bodyObject1->getMass();
					std::cout << "DIFF: " << std::string(diff) << " MAG: " << diff.getMagnitude() << std::endl;
				}*/
				//if (abs(totalForce) > 1.0)
				//	std::cout << "geom1: " << std::hex << geom1->getCategory() << " geom2: " << geom2->getCategory() << " normal: " << std::string(contactNormal) << std::endl;

				/*if (geom1->getCategory() == 2)
					dir *= TGen::Vector3::DotProduct(force1.getNormalized(), contactNormal);
				
				if (geom2->getCategory() == 2)
					dir *= TGen::Vector3::DotProduct(force2.getNormalized(), contactNormal);
				*/
				//std::cout << dir << " DIR" << std::endl;
				
				//totalForce -= 10.0;
				//totalForce = std::max(totalForce, 0.0f);
				//totalForce *= dir;
				
				//if (totalForce > 2.0)
				//	std::cout << "TOTAL FORCE: " << totalForce << std::endl;
				
				if (contactNormal.y >= -0.5)	// TODO: hurtable, force ska vara mindre för den som får mindre slag
					geom1->onCollisionForce(totalForce, body1OnGround, geom2);
				if (contactNormal.y <= -0.5)
					geom2->onCollisionForce(totalForce, body2OnGround, geom1);
				
				//if (totalForce > 0.1)
					//std::cout << "HURT: " << totalForce << std::endl;
			//}

			collisionEvents.push_back(contacts[i]);
	
			if (geom1 && geom1->getCalculateVelocity()) {
				contacts[i].surface.mode |= (dContactMotion1 | dContactMotion2 | dContactMotionN | dContactFDir1);
				
				const dReal * normal = contacts[i].geom.normal;
				const dReal velocity[] = {geom1->getVelocity().x, geom1->getVelocity().y, geom1->getVelocity().z};
				
				dVector3 fdir1, fdir2;
				dPlaneSpace(normal, fdir1, fdir2);
				contacts[i].fdir1[0] = fdir1[0];
				contacts[i].fdir1[1] = fdir1[1];
				contacts[i].fdir1[2] = fdir1[2];
			
				contacts[i].surface.motion1 = dDOT(velocity, fdir1);
				contacts[i].surface.motion2 = -1.0 * dDOT(velocity, fdir2);
				contacts[i].surface.motionN = -1.0 * dDOT(velocity, normal);
			}

			if (geom2 && geom2->getCalculateVelocity()) {
				contacts[i].surface.mode |= (dContactMotion1 | dContactMotion2 | dContactMotionN | dContactFDir1);
				
				const dReal * normal = contacts[i].geom.normal;
				const dReal velocity[] = {geom2->getVelocity().x, geom2->getVelocity().y, geom2->getVelocity().z};
				
				dVector3 fdir1, fdir2;
				dPlaneSpace(normal, fdir1, fdir2);
				contacts[i].fdir1[0] = fdir1[0];
				contacts[i].fdir1[1] = fdir1[1];
				contacts[i].fdir1[2] = fdir1[2];
				
				contacts[i].surface.motion1 = dDOT(velocity, fdir1);
				contacts[i].surface.motion2 = dDOT(velocity, fdir2);
				contacts[i].surface.motionN = dDOT(velocity, normal);
			}

			
			if ((!geom1 || (geom1 && geom1->onCollision(geom2, o1, contacts[i]))) && (!geom2 || (geom2 && geom2->onCollision(geom1, o2, contacts[i])))) {
				dJointID contactJoint = dJointCreateContact(worldId, contactGroup, &contacts[i]);
				dJointAttach(contactJoint, body1, body2);
			}
		}
	}
	
}

dWorldID TGen::Engine::Physics::Subsystem::getWorldId() {
	return worldId;
}

dSpaceID TGen::Engine::Physics::Subsystem::getSpaceId() {
	return mainSpace;
}

void TGen::Engine::Physics::Subsystem::triggerCollisionEvents() {
	for (int i = 0; i < collisionEvents.size(); ++i) {
		const dContact & contact = collisionEvents[i];
		
		TGen::Engine::Physics::Geom * geom1 = static_cast<TGen::Engine::Physics::Geom *>(dGeomGetData(contact.geom.g1));
		TGen::Engine::Physics::Geom * geom2 = static_cast<TGen::Engine::Physics::Geom *>(dGeomGetData(contact.geom.g2));		
		
		geom1->postCollision(geom2, contact.geom.g2, contact);
		geom2->postCollision(geom1, contact.geom.g1, contact);

	}
}


