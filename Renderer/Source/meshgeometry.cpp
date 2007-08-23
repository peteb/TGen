/*
 *  meshgeometry.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/14/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "meshgeometry.h"
#include "meshsource.h"

TGen::MeshGeometry::MeshGeometry(const std::string & mesh)
	: meshName(mesh)
	, mesh(NULL)
{
}

TGen::MeshGeometry::~MeshGeometry() {
	for (int i = 0; i < observers.size(); ++i)
		observers[i]->onRemoved(*this);
}

void TGen::MeshGeometry::linkMesh(TGen::MeshSource & source) {
	mesh = source.getMesh(meshName);
}

void TGen::MeshGeometry::unlinkMesh() {
	mesh = NULL;
}

TGen::Mesh * TGen::MeshGeometry::getMesh() const {
	return mesh;
}

std::string TGen::MeshGeometry::getMeshName() const {
	return meshName;
}

void TGen::MeshGeometry::preRender(TGen::Renderer & renderer) const {
	testLinkedMesh("preRender");	
}

void TGen::MeshGeometry::render(TGen::Renderer & renderer) const {
	testLinkedMesh("render");
}

void TGen::MeshGeometry::update(const TGen::Camera & camera, scalar distance, scalar time) {
	testLinkedMesh("update");
}

TGen::Vector3 TGen::MeshGeometry::getMax() const {
	return TGen::Vector3(0.0f, 0.0f, 0.0f);
}

TGen::Vector3 TGen::MeshGeometry::getMin() const {
	return TGen::Vector3(0.0f, 0.0f, 0.0f);
}

TGen::Vector3 TGen::MeshGeometry::getOrigin() const {
	return TGen::Vector3(0.0f, 0.0f, 0.0f);
}

void TGen::MeshGeometry::attachedToObserver(MeshGeometryObserver * observer) {
	observers.push_back(observer);
}

void TGen::MeshGeometry::detachedFromObserver(MeshGeometryObserver * observer) {
	ObserverList::iterator iter = std::find(observers.begin(), observers.end(), observer);
	if (iter != observers.end())
		observers.erase(iter);
}

void TGen::MeshGeometry::testLinkedMesh(const std::string & place) const {
	if (!mesh)
		throw TGen::RuntimeException("MeshGeometry::" + place, "mesh '" + meshName + "' not linked!");
}

