/*
 *  raygeom.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/27/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "physics/geom.h"
#include "physics/body.h"
#include "physics/raygeom.h"
#include <tgen_math.h>

TGen::Engine::Physics::RayGeom::RayGeom(const std::string & name, scalar length, dSpaceID space)
	: TGen::Engine::Physics::Geom(name)
	, collided(false)
{
	dGeomID newGeom = dCreateRay(space, length);
	
	setGeomId(newGeom);
}

void TGen::Engine::Physics::RayGeom::setOrientation(const TGen::Matrix3x3 & orientation) {
	TGen::Matrix3x3 fixedRotation = orientation;
	fixedRotation.setZ(-fixedRotation.getZ());
	
	TGen::Engine::Physics::Geom::setOrientation(fixedRotation);
}

// flagga geom för deletion
// / flagga entity för deletion

bool TGen::Engine::Physics::RayGeom::onCollision(TGen::Engine::Physics::Geom * with, dGeomID id, const dContact & contactInfo) {
	//dGeomDestroy(geomId);
	//geomId = NULL;
	if (collided)
		return false;
	
	
	TGen::Engine::Physics::Body * body = with->getBody();
	
	if (!body)
		return false;
	
	TGen::Vector3 collisionAt(contactInfo.geom.pos[0], contactInfo.geom.pos[1], contactInfo.geom.pos[2]);
	TGen::Vector3 collisionDir(contactInfo.geom.normal[0], contactInfo.geom.normal[1], contactInfo.geom.normal[2]);
	
	body->addForceWorld(collisionAt, collisionDir * 5000000.0); 
		
	std::cout << "HEYYYY HIT" << std::endl;
	
	collided = true;
	
	return false;
}
