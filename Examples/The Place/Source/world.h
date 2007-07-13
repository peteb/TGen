/*
 *  world.h
 *  The Place
 *
 *  Created by Peter Backman on 7/9/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_THEPLACE_WORLD_H
#define _TGEN_THEPLACE_WORLD_H

#define __TGEN__
#include <ode/ode.h>
#include <tgen_math.h>
#include <tgen_graphics.h>
#include "level.h"

class Player;

class World {
public:	
	World();
	~World();
	
	void Update(scalar dt);
	void Render(TGen::Renderer & renderer);
	void LoadData(TGen::Renderer & renderer, ResourceManager & resources);
	Player * getPlayer();
	
	Plane * CreatePlane(const TGen::Vector3 & position, const TGen::Vector3 & direction, TGen::Material * material);
	Player * CreatePlayer(const TGen::Vector3 & position, const TGen::Vector3 & direction);
	
	
	dWorldID worldId;
	dJointGroupID contactJointsGroupId;
	dSpaceID spaceId;
	
private:
	
	TGen::Vector3 gravity;

	Player * player;
	Level * level;
	scalar lastWorldStep, timeNow;
};

void NearCallback(void * data, dGeomID o1, dGeomID o2);
#endif // !_TGEN_THEPLACE_WORLD_H
