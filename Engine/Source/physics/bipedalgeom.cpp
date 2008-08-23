/*
 *  bipedalgeom.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/1/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "physics/bipedalgeom.h"
#include "physics/body.h"
#include "entity.h"
#include <iostream>
#include <tgen_math.h>

TGen::Engine::Symbol TGen::Engine::Physics::BipedalGeom::symbolMakeSloppy = TGen::Engine::getUniqueSymbol("setSloppy");

TGen::Engine::Physics::BipedalGeom::BipedalGeom(const std::string & name, dSpaceID space, scalar capRadius, scalar length)
	: TGen::Engine::Physics::Geom(name)
	, capRadius(capRadius)
	, length(length)
	, sloppy(false)
{
	dGeomID newGeom = dCreateCCylinder(space, capRadius, length);
	setGeomId(newGeom);
	
	
	//raylegs = dCreateRay(space, 1.0);

	//dGeomRaySetLength(raylegs, 3.0);
	
	//dGeomSetData(raylegs, reinterpret_cast<void *>(this));
}

void TGen::Engine::Physics::BipedalGeom::preStep() {
	//TGen::Engine::Physics::Geom::preStep();
	if (!sloppy) {
		TGen::Engine::Physics::Geom::preStep();
	

		setOrientation(TGen::Rotation(TGen::Vector3(1.0f, 0.0f, 0.0f),
												TGen::Vector3(0.0f, 0.0f, -1.0f),
												TGen::Vector3(0.0f, 1.0f, 0.0f)));
	}
	
	/*const dReal * linearVel = dBodyGetLinearVel(attachedTo->getBodyId());
	TGen::Vector3 offset(linearVel[0], 0.0, linearVel[2]);
	if (offset.getMagnitude() > 1.0)
		offset.normalize();
	
	TGen::Vector3 pos = attachedTo->getPosition() + offset * 0.1;
	dGeomRaySet(raylegs, pos.x, pos.y, pos.z, 0.0, -1.0, 0.0);*/
}

void TGen::Engine::Physics::BipedalGeom::postStep() {
	if (!sloppy)
		TGen::Engine::Physics::Geom::postStep();
	/*setOrientation(TGen::Rotation(TGen::Vector3(1.0f, 0.0f, 0.0f),
											TGen::Vector3(0.0f, 0.0f, -1.0f),
											TGen::Vector3(0.0f, 1.0f, 0.0f)));
						
		*/				
						
						
						
//						TGen::Matrix4x4::RotationX(TGen::Radian(TGen::HALF_PI)));

}

void TGen::Engine::Physics::BipedalGeom::link(const TGen::Engine::ComponentLinker & linker) {
	if (!sloppy)
		TGen::Engine::Physics::Geom::link(linker);
	
	//TGen::Engine::Physics::Body * attachTo = dynamic_cast<TGen::Engine::Physics::Body *>(entity.getComponent(bodyComponent));
	//dGeomSetBody(raylegs, attachTo->getBodyId());
}

bool TGen::Engine::Physics::BipedalGeom::onCollision(TGen::Engine::Physics::Geom * with, dGeomID id, const dContact & contactInfo) {
	if (id == raylegs && with != this) {
		
		//scalar diff = std::max(3.0 - contactInfo.geom.depth, 0.0);
		//std::cout << "RAYLEGS " << contactInfo.geom.depth << " .. " << diff << std::endl;

		//attachedTo->setOnFloor(true);
		
		//if (diff > 0.0)
		//	attachedTo->addForce(TGen::Vector3(0.0f, diff, 0.0f));
		
		return false;
	}
	
	return true;
}

void TGen::Engine::Physics::BipedalGeom::trigger(TGen::Engine::TriggerContext & context, TriggerMode mode) {
	TGen::Engine::Symbol symbolNum = context.getRegister<TGen::Engine::Symbol>(0);
	
	if (symbolNum == symbolMakeSloppy) {
		int isSloppy = context.getParameter<int>(2);
		sloppy = isSloppy;
	}
	else {
		TGen::Engine::Physics::Geom::trigger(context, mode);
	}
	
}

