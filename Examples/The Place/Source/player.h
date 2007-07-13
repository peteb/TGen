/*
 *  player.h
 *  The Place
 *
 *  Created by Peter Backman on 7/9/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_THEPLACE_PLAYER_H
#define _TGEN_THEPLACE_PLAYER_H

#include <tgen_math.h>
#define __TGEN__
#include <ode/ode.h>

class Player {
public:
	Player(const TGen::Vector3 & position, const TGen::Vector3 & direction, dGeomID geomId, dBodyID bodyId);
	
	void Update(scalar dt);
	const TGen::Vector3 & getPosition() const;
	const TGen::Vector3 & getDirection() const;
	
	
	bool moveForward, spinRight, spinLeft;
	float lookDir;
	
private:
	TGen::Vector3 position, direction;
	dGeomID geomId;
	dBodyID bodyId;
};

#endif // !_TGEN_THEPLACE_PLAYER_H
