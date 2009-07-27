/*
 *  q3map.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/27/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include "q3bsp/q3map.h"

TGen::Engine::Q3Map::Q3Map(const std::string & name)
	: TGen::SceneNode(name)
{
}

bool TGen::Engine::Q3Map::fillFaces(TGen::RenderList & list, const TGen::Camera & camera) const {
	
	return true;
}
