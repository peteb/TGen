/*
 *  surface.cpp
 *  The Quaker
 *
 *  Created by Peter Backman on 7/18/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "surface.h"

Surface::Surface(const std::string & materialName, Geometry * geometry, SceneNode * node) 
	: materialName(materialName)
	, geometry(geometry)
	, material(NULL)
	, sceneNode(node)
{
	
}

Surface::Surface(TGen::Material * material, Geometry * geometry, SceneNode * node)
	: material(material)
	, geometry(geometry)
	, sceneNode(node)
{
	
}

Surface::~Surface() {
	
}


TGen::Material * Surface::getMaterial() const {
	return material;
}

void Surface::Link(SurfaceLinker & linker) {
	if (!materialName.empty())
		material = linker.getMaterial(materialName);
}

void Surface::PrepareRender(TGen::Renderer & renderer) const {
	geometry->PrepareRender(renderer);
}

void Surface::Render(TGen::Renderer & renderer) const {
	geometry->Render(renderer);
}

TGen::Vector3 Surface::getMin() const {
	return geometry->getMin();
}

TGen::Vector3 Surface::getMax() const {
	return geometry->getMax();
}

SceneNode * Surface::getSceneNode() const {
	return sceneNode;
}
