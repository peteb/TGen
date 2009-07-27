/*
 *  q3map.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/27/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include "q3bsp/q3map.h"
#include "q3bsp/q3mapmodel.h"

TGen::Engine::Q3Map::Q3Map(const std::string & name)
	: TGen::SceneNode(name)
{
}

bool TGen::Engine::Q3Map::fillFaces(TGen::RenderList & list, const TGen::Camera & camera) const {
	for (int i = 0; i < models.size(); ++i)
		models[i]->fillFaces(list, NULL, this);
	
	return true;
}

void TGen::Engine::Q3Map::linkMaterial(TGen::MaterialSource & source) {
	for (int i = 0; i < models.size(); ++i)
		models[i]->linkMaterial(source);
}

void TGen::Engine::Q3Map::instantiate(TGen::VertexDataSource & source) {
	for (int i = 0; i < models.size(); ++i)
		models[i]->instantiate(source);
}


void TGen::Engine::Q3Map::addModel(TGen::Engine::Q3MapModel * model) {
	models.push_back(model);
}

