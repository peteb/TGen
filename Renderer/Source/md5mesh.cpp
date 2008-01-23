/*
 *  md5mesh.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 1/19/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "md5mesh.h"

TGen::MD5::Mesh::Mesh(const std::string & name) 
	: TGen::Mesh(name)
{
	
}

TGen::MD5::Mesh::~Mesh() {
	
}

void TGen::MD5::Mesh::preRender(TGen::Renderer & renderer) const {
	
}

void TGen::MD5::Mesh::render(TGen::Renderer & renderer) const {
	
}

void TGen::MD5::Mesh::update(const TGen::Camera & camera, scalar distance, scalar time) {
	
}

TGen::Vector3 TGen::MD5::Mesh::getMax() const {
	return TGen::Vector3(1.0f, 1.0f, 1.0f);
}

TGen::Vector3 TGen::MD5::Mesh::getMin() const {
	return TGen::Vector3(-1.0f, -1.0f, -1.0f);
}

TGen::Vector3 TGen::MD5::Mesh::getOrigin() const {
	return TGen::Vector3(0.0f, 0.0f, 0.0f);
}

std::string TGen::MD5::Mesh::getDefaultMaterial() const {
	return "";
}

