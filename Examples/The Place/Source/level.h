/*
 *  level.h
 *  The Place
 *
 *  Created by Peter Backman on 7/9/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_THEPLACE_LEVEL_H
#define _TGEN_THEPLACE_LEVEL_H

#include <tgen_graphics.h>
#include "plane.h"

class World;

class Level {
public:
	Level(World & world);
	~Level();
	
	void Render(TGen::Renderer & renderer);
	void Update(scalar dt);
	void LoadData(TGen::Renderer & renderer, ResourceManager & resources);
	
private:
	Plane * nicePlane;
	World & world;
};

#endif // !_TGEN_THEPLACE_LEVEL_H
