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

class World {
public:	
	World();
	~World();
	
	void Update(scalar dt);
	void Render(TGen::Renderer & renderer);
	void LoadData(TGen::Renderer & renderer, ResourceManager & resources);
	
	Plane * CreatePlane(const TGen::Vector3 & position, const TGen::Vector3 & direction, TGen::Material * material);
	
private:
	dWorldID worldId;
	TGen::Vector3 gravity;

	Level * level;
	scalar lastWorldStep, timeNow;
};


#endif // !_TGEN_THEPLACE_WORLD_H
