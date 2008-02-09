/*
 *  md3animmesh.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/3/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "md3animmesh.h"

TGen::MD3::AnimationFrame::AnimationFrame(const std::string & name)
	: name(name)
{
}

TGen::MD3::AnimatingMesh::AnimatingMesh(const std::string & materialName, int numVerts)
	: materialName(materialName)
	, numVertices(numVerts)
{
}

TGen::MD3::AnimatingMesh::~AnimatingMesh() {

}


void TGen::MD3::AnimatingMesh::addAnimationFrame(TGen::MD3::AnimationFrame * frame) {
	frames.push_back(frame);
}

std::string TGen::MD3::AnimatingMesh::getMaterialName() const {
	return materialName;
}

int TGen::MD3::AnimatingMesh::getNumAnimationFrames() const {
	return frames.size();
}

TGen::MD3::AnimationFrame const & TGen::MD3::AnimatingMesh::getAnimationFrame(int num) const {
	return *frames[num];
}

int TGen::MD3::AnimatingMesh::getNumVertices() const {
	return numVertices;
}

int TGen::MD3::AnimatingMesh::getNumIndices() const {
	return indices.size();
}

void TGen::MD3::AnimatingMesh::reserveIndices(uint num) {
	indices.reserve(num);
}

void TGen::MD3::AnimatingMesh::addIndex(const TGen::MD3::IndexDecl::Type & index) {
	indices.push_back(index);
}

void TGen::MD3::AnimatingMesh::reserveTexcoords(uint num) {
	texcoords.reserve(num);
}

void TGen::MD3::AnimatingMesh::addTexcoord(const TGen::MD3::TexCoordDecl::Type & texcoord) {
	texcoords.push_back(texcoord);
}

TGen::MD3::TexCoordDecl::Type TGen::MD3::AnimatingMesh::getTexcoord(uint num) const {
	return texcoords[num];
}

TGen::MD3::IndexDecl::Type const * TGen::MD3::AnimatingMesh::getIndexArray() const {
	return &indices[0];
}

