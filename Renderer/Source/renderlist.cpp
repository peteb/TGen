/*
 *  renderlist.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/14/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include <tgen_graphics.h>
#include "renderlist.h"
#include "face.h"
#include "camera.h"
#include "geometry.h"

TGen::RenderList::RenderList() {
	opaqueFaces.reserve(100000);
	transparentFaces.reserve(10000);
}

TGen::RenderList::~RenderList() {
	
}

TGen::Sorter::Sorter(const TGen::Vector3 & position)
	: position(position)
{
		
}


bool TGen::Sorter::operator() (const TGen::Face * face1, const TGen::Face * face2) {
	TGen::Vector3 origin1 = face1->getSceneNode()->getWorldPosition() + face1->getGeometry()->getOrigin();
	TGen::Vector3 origin2 = face1->getSceneNode()->getWorldPosition() + face2->getGeometry()->getOrigin();

	return (origin1 - position).getMagnitude() > (origin2 - position).getMagnitude();
	// TODO: spara undan origin i world för face!

}

void TGen::RenderList::addFace(const TGen::Face * face) {
	if (!face->getMaterial())
		throw TGen::RuntimeException("RenderList::addFace", "no material linked!");
	
	if (face->getMaterial()->getSortLevel() == TGen::MaterialSortOpaque)
		opaqueFaces.push_back(face);
	else
		transparentFaces.push_back(face);
}

void TGen::RenderList::clear() {
	
}

void TGen::RenderList::sort(const TGen::Vector3 & position) {
	std::sort(transparentFaces.begin(), transparentFaces.end(), TGen::Sorter(position));
	
}

void TGen::RenderList::render(TGen::Renderer & renderer, const TGen::Camera & camera) {
	
}

void TGen::RenderList::print() {
	std::cout << "opaque: " << std::endl;
	for (int i = 0; i < opaqueFaces.size(); ++i)
		std::cout << opaqueFaces[i] << std::endl;
	
	std::cout << "transparent: " << std::endl;
	for (int i = 0; i < transparentFaces.size(); ++i)
		std::cout << transparentFaces[i] << "  " << std::string(transparentFaces[i]->getGeometry()->getOrigin()) << std::endl;
}
