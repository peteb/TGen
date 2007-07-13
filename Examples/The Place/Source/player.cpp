/*
 *  player.cpp
 *  The Place
 *
 *  Created by Peter Backman on 7/9/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "player.h"
#include <iostream>

Player::Player(const TGen::Vector3 & position, const TGen::Vector3 & direction, dGeomID geomId, dBodyID bodyId) 
	: position(position), direction(direction), geomId(geomId), bodyId(bodyId), moveForward(false), spinRight(false), spinLeft(false), lookDir(false)
{
	
}

void Player::Update(scalar dt) {
	const dReal * pos = dBodyGetPosition(bodyId);
	
	position.x = pos[0];
	position.y = pos[1];
	position.z = pos[2];
	
	if (moveForward) {
		TGen::Vector3 hej = direction * 2.6f * dt;

		dBodyEnable(bodyId);
		
		dBodySetForce(bodyId, hej.x, hej.y, hej.z);
		//dBodySetLinearVel(bodyId, hej.x, hej.y, hej.z);
	}
	else {
		
//		dBodySetLinearVel(bodyId, 0.0f, 0.0f, 0.0f);
	}

	if (spinLeft)
		lookDir += dt * 2.0f;
	
	if (spinRight)
		lookDir -= dt * 2.0f;
	
	direction.x = sin(lookDir);
	direction.z = cos(lookDir);
	
	dMatrix3 ident;
	dRSetIdentity(ident);
	dBodySetRotation(bodyId, ident);
	dBodySetAngularVel(bodyId, 0.0f, 0.0f, 0.0f);
	dBodySetTorque(bodyId, 0.0f, 0.0f, 0.0f);
	
	//std::cout << "player: " << std::string(position) << std::endl;
}

const TGen::Vector3 & Player::getPosition() const {
	return position;
}

const TGen::Vector3 & Player::getDirection() const {
	return direction;
}
