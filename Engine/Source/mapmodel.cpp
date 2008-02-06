/*
 *  mapmodel.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/30/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "mapmodel.h"
#include <tgen_renderer.h>
#include "mapsurface.h"
#include "map.h"

TGen::Engine::MapModel::MapModel(const std::string & name, TGen::Engine::Map * map)
	: name(name)
	, map(map)
{
	
}

std::string TGen::Engine::MapModel::getName() const {
	return name;
}

void TGen::Engine::MapModel::addSurface(TGen::Engine::MapSurface * surface) {
	surfaces.push_back(surface);
	faces.push_back(new TGen::Face(surface, surface->getMaterialName(), map));
}

bool TGen::Engine::MapModel::fillFaces(TGen::RenderList & list, const TGen::Camera & camera) const {
	//for (int i = 0; i < faces.size(); ++i)
	//	list.addFace(faces[i]);
	
	throw TGen::RuntimeException("MapModel::fillFaces", "BARG");
	
	return true;
}

void TGen::Engine::MapModel::linkMaterials(TGen::MaterialSource & source) {
	for (int i = 0; i < faces.size(); ++i)
		faces[i]->linkMaterial(source);
}

void TGen::Engine::MapModel::createVertexData(TGen::VertexDataSource & source) {
	for (int i = 0; i < surfaces.size(); ++i)
		surfaces[i]->createVertexData(source);
}
