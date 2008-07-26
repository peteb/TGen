/*
 *  worldinfo.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/24/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "info/worldinfo.h"
#include "playercontroller.h"
#include "world.h"

TGen::Engine::Symbol TGen::Engine::Info::WorldInfo::symbolSetGravity = TGen::Engine::getUniqueSymbol("setGravity");
TGen::Engine::Symbol TGen::Engine::Info::WorldInfo::symbolGetGravity = TGen::Engine::getUniqueSymbol("getGravity");

TGen::Engine::Info::WorldInfo::WorldInfo(const std::string & name, TGen::Engine::World * world)
	: TGen::Engine::Component(name)
	, world(world)
{
}

void TGen::Engine::Info::WorldInfo::linkGlobally(TGen::Engine::EntityList & entities, TGen::Engine::Entity & entity) {
	playerController.link(entities, entity);
}

void TGen::Engine::Info::WorldInfo::trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode) {
	TGen::Engine::Symbol symbolNum = context.getFunctionSymbol();
		
	if (!world)
		return;
	
	if (symbolNum == symbolSetGravity) {
		if (context.numParameters == 3) {
			TGen::Vector3 vector(*context.getRegister<scalar *>(2), *context.getRegister<scalar *>(3), *context.getRegister<scalar *>(4));
			std::cout << "GRAVMOO: " << std::string(vector) << std::endl;
			//exit(10);
		
			world->physicsSubsystem.setGravity(vector);
		}
		else if (context.numParameters == 1) {
			scalar * vector = *context.getRegister<scalar **>(2);
			
			TGen::Vector3 gravity(vector[0], vector[1], vector[2]);
			
			std::cout << "GRAV: " << std::string(gravity) << std::endl;
		//	exit(10);
			
			world->physicsSubsystem.setGravity(gravity);
		}
		
	}
	else if (symbolNum == symbolGetGravity) {
		TGen::Vector3 gravity = world->physicsSubsystem.getGravity();

		static scalar gravityVector[3];
		
		gravityVector[0] = gravity.x;
		gravityVector[1] = gravity.y;
		gravityVector[2] = gravity.z;
		
		*context.getRegister<scalar **>(context.getReturnRegister()) = gravityVector;
	}
	else {
		TGen::Engine::Component::trigger(context, mode);
	}
}

void TGen::Engine::Info::WorldInfo::setPlayerController(const std::string & name) {
	playerController.set(name);
}

TGen::Engine::PlayerController * TGen::Engine::Info::WorldInfo::getPlayerController() {
	return &*playerController;
}
