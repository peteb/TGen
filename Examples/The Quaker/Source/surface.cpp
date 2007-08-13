/*
 *  surface.cpp
 *  The Quaker
 *
 *  Created by Peter Backman on 7/18/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "surface.h"

Surface::Surface(const std::string & materialName, Geometry * geometry) 
	: materialName(materialName)
	, geometry(geometry)
	, material(NULL)
	, sceneNode(NULL)
{
	
}

Surface::Surface(TGen::Material * material, Geometry * geometry)
	: material(material)
	, geometry(geometry)
	, sceneNode(NULL)
{
	
}

Surface::~Surface() {
	
}

void Surface::Attached(SceneNode * parent) {
	if (sceneNode)
		throw TGen::RuntimeException("Surface::Attached", "surface already attached!");
	
	sceneNode = parent;
}

TGen::Material * Surface::getMaterial() const {
	return material;
}

void Surface::Link(SurfaceLinker & linker) {
	if (!materialName.empty())
		material = linker.getMaterial(materialName);
}

void Surface::preRender(TGen::Renderer & renderer) const {
	if (geometry)
		geometry->preRender(renderer);
}

void Surface::render(TGen::Renderer & renderer) const {
	if (geometry)
		geometry->render(renderer);
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

Geometry * Surface::getGeometry() const {
	return geometry;
}

