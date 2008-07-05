/*
 *  face_new.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/3/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "face_new.h"
#include "scenenode.h"

TGen::NewFace::NewFace(TGen::NewMeshInstance * mesh, TGen::Material * material, TGen::SceneNode const * sceneNode, const TGen::ModelRenderProperties * renderProperties)
	: mesh(mesh)
	, material(material)
	, sceneNode(sceneNode)
	, renderProperties(renderProperties)
{

}

TGen::NewMeshInstance * TGen::NewFace::getMesh() const {
	return mesh;
}

TGen::Material * TGen::NewFace::getMaterial() const {
	return material;
}

TGen::SceneNode const * TGen::NewFace::getSceneNode() const {
	return sceneNode;
}

TGen::Vector3 TGen::NewFace::getWorldOrigin() const {
	return sceneNode->getTransform().getOrigin();
}

void TGen::NewFace::setSceneNode(const TGen::SceneNode * node) {
	sceneNode = node;
}

void TGen::NewFace::setMaterial(TGen::Material * material) {
	this->material = material;
}

const TGen::ModelRenderProperties * TGen::NewFace::getRenderProperties() const {
	return renderProperties;
}
