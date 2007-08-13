/*
 *  face.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "face.h"
#include "materialsource.h"

TGen::Face::Face(TGen::Geometry * geometry, const std::string & materialName)
	: material(NULL)
	, materialName(materialName)
	, geometry(geometry)
	, sceneNode(NULL)
{
		
}

TGen::Face::~Face() {
	
}

TGen::SceneNode * TGen::Face::getSceneNode() const {
	return sceneNode;
}

void TGen::Face::attached(TGen::SceneNode * node) {
	sceneNode = node;
}

void TGen::Face::linkMaterial(TGen::MaterialSource & source) {
	if (!materialName.empty())
		material = source.getMaterial(materialName);
}

void TGen::Face::unlinkMaterial() {
	material = NULL;
}

TGen::Geometry * TGen::Face::getGeometry() const {
	return geometry;
}

TGen::Material * TGen::Face::getMaterial() const {
	return material;
}

std::string TGen::Face::getMaterialName() const {
	return materialName;
}
