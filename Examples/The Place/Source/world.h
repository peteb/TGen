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

class World {
public:	
	World();
	~World();
	
	void Update();
	void Render();
	
private:
	dWorldID worldId;
	TGen::Vector3 gravity;
};


#endif // !_TGEN_THEPLACE_WORLD_H
