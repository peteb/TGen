/*
 *  bipedalgeom.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/1/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "physics/bipedalgeom.h"
#include <iostream>
#include <tgen_math.h>

TGen::Engine::Physics::BipedalGeom::BipedalGeom(const std::string & name, dSpaceID space, scalar capRadius, scalar length)
	: TGen::Engine::Physics::Geom(name)
	, capRadius(capRadius)
	, length(length)
{
	dGeomID newGeom = dCreateCCylinder(space, capRadius, length);
	setGeomId(newGeom);
	
}

void TGen::Engine::Physics::BipedalGeom::preStep() {
	//TGen::Engine::Physics::Geom::preStep();
	TGen::Engine::Physics::Geom::preStep();
	setOrientation(TGen::Rotation(TGen::Vector3(1.0f, 0.0f, 0.0f),
											TGen::Vector3(0.0f, 0.0f, -1.0f),
											TGen::Vector3(0.0f, 1.0f, 0.0f)));
	
	
}

void TGen::Engine::Physics::BipedalGeom::postStep() {
	TGen::Engine::Physics::Geom::postStep();
	/*setOrientation(TGen::Rotation(TGen::Vector3(1.0f, 0.0f, 0.0f),
											TGen::Vector3(0.0f, 0.0f, -1.0f),
											TGen::Vector3(0.0f, 1.0f, 0.0f)));
						
		*/				
						
						
						
//						TGen::Matrix4x4::RotationX(TGen::Radian(TGen::HALF_PI)));

}