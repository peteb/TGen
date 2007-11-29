/*
 *  lightlist.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 11/18/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "lightlist.h"
#include "light.h"

TGen::Engine::LightList::LightList(int num) {
	lights.reserve(num);
}

void TGen::Engine::LightList::addLight(TGen::Engine::Light * light) {
	lights.push_back(light);
}

TGen::Engine::Light * TGen::Engine::LightList::getLight(int id) const {
	return lights[id];
}

int TGen::Engine::LightList::getNumLights() const {
	return lights.size();
}

void TGen::Engine::LightList::clear() {
	lights.clear();
}
