/*
 *  basicrenderlist.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/14/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "basicrenderlist.h"
#include "face.h"
#include "geometry.h"
#include "scenenode.h"
#include "camera.h"
#include <tgen_graphics.h>

TGen::BasicRenderList::BasicRenderList() {
	opaqueFaces.reserve(10000);
	transparentFaces.reserve(1000);
}

TGen::BasicRenderList::~BasicRenderList() {}

void TGen::BasicRenderList::addFace(const TGen::Face * face) {
	if (!face->getMaterial())
		throw TGen::RuntimeException("BasicRenderList::addFace", "no material linked!");
	
	if (face->getMaterial()->getSortLevel() == TGen::MaterialSortOpaque)
		opaqueFaces.push_back(face);
	else
		transparentFaces.push_back(face);
}

void TGen::BasicRenderList::clear() {
	opaqueFaces.clear();
	transparentFaces.clear();
}

void TGen::BasicRenderList::sort(const TGen::Camera & camera) {
	if (needSorting()) {
		calculateCameraDistance(transparentFaces, camera);
		std::sort(transparentFaces.begin(), transparentFaces.end(), TGen::BasicRenderList::Sorter());
	}
}

void TGen::BasicRenderList::render(TGen::Renderer & renderer, const TGen::Camera & camera) {
	renderList(opaqueFaces, renderer, camera);
	renderList(transparentFaces, renderer, camera);
}

void TGen::BasicRenderList::renderList(TGen::BasicRenderList::SortedFaceList & list, TGen::Renderer & renderer, const TGen::Camera & camera) {
	TGen::Matrix4x4 baseMat = camera.getTransform();
	renderer.setTransform(TGen::TransformProjection, camera.getProjection());

	TGen::SceneNode * lastNode = NULL;

	for (int i = 0; i < list.size(); ++i) {
		const TGen::Face * face = list[i].face;
		TGen::SceneNode * node = face->getSceneNode();
		
		if (node && lastNode != node) {
			renderer.setTransform(TGen::TransformWorldView, baseMat * node->getTransform());
			lastNode = node;
		}
		
		TGen::Material * material = face->getMaterial();
		material->render(renderer, *face->getGeometry(), "default", 9, NULL);
	}
}

void TGen::BasicRenderList::calculateCameraDistance(SortedFaceList & list, const TGen::Camera & camera) {
	for (int i = 0; i < list.size(); ++i) {
		list[i].distanceToCamera = (list[i].face->getWorldOrigin() - camera.getWorldPosition()).getMagnitude();
	}
}

void TGen::BasicRenderList::print() {
	std::cout << "opaque: " << std::endl;
	for (int i = 0; i < opaqueFaces.size(); ++i)
		std::cout << opaqueFaces[i].face << std::endl;
	
	std::cout << "transparent: " << std::endl;
	for (int i = 0; i < transparentFaces.size(); ++i)
		std::cout << transparentFaces[i].face << "  " << std::string(transparentFaces[i].face->getWorldOrigin()) << " - " << transparentFaces[i].distanceToCamera << std::endl;
}

bool TGen::BasicRenderList::needSorting() {
	return true;
}

TGen::BasicRenderList::Sorter::Sorter() {
}

TGen::BasicRenderList::SortedFace::SortedFace(const TGen::Face * face)
	: face(face)
	, distanceToCamera(0.0f)
{
}

bool TGen::BasicRenderList::Sorter::operator() (const TGen::BasicRenderList::SortedFace & face1, const TGen::BasicRenderList::SortedFace & face2) {
	return (face1.distanceToCamera > face2.distanceToCamera);
}


