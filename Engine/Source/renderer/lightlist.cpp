/*
 *  lightlist.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 11/18/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "renderer/lightlist.h"
#include "light.h"

TGen::Engine::LightList::LightList(int num) {
	lights.reserve(num);
}

TGen::Engine::LightList::~LightList() {
	for (LightMap::iterator iter = lightsByMaterials.begin(); iter != lightsByMaterials.end(); ++iter) {
		delete iter->second;
	}	
}

void TGen::Engine::LightList::addLight(TGen::Engine::Light * light) {
	lights.push_back(light);
	
	LightMap::iterator iter = lightsByMaterials.find(light->getMaterial());
	if (iter == lightsByMaterials.end()) {
		LightArray * newList = new LightArray;
		newList->reserve(100);
		
		lightsByMaterials.insert(LightMap::value_type(light->getMaterial(), newList));
		newList->push_back(light);
	}
	else {
		iter->second->push_back(light);
	}
}

TGen::Engine::Light * TGen::Engine::LightList::getLight(int id) const {
	return lights[id];
}

int TGen::Engine::LightList::getNumLights() const {
	return lights.size();
}

void TGen::Engine::LightList::clear() {
	lights.clear();
	
	for (LightMap::iterator iter = lightsByMaterials.begin(); iter != lightsByMaterials.end(); ++iter) {
		iter->second->clear();
	}
}

TGen::Engine::LightList::LightMap & TGen::Engine::LightList::getLightsByMaterial() {
	return lightsByMaterials;
}
