/*
 *  md3mesh.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/23/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "md3mesh.h"

TGen::MD3::Mesh::Mesh(const std::string & name)
	: TGen::Mesh(name)
{
		
}

TGen::MD3::Mesh::~Mesh() {
	
}

void TGen::MD3::Mesh::preRender(TGen::Renderer & renderer) const {

}

void TGen::MD3::Mesh::render(TGen::Renderer & renderer) const {

}

void TGen::MD3::Mesh::update(const TGen::Camera & camera, scalar distance, scalar time) {
	
}

TGen::Vector3 TGen::MD3::Mesh::getMax() const {
	return TGen::Vector3(1.0f, 1.0f, 1.0f);
}

TGen::Vector3 TGen::MD3::Mesh::getMin() const {
	return TGen::Vector3(-1.0f, -1.0f, -1.0f);
}

TGen::Vector3 TGen::MD3::Mesh::getOrigin() const {
	return TGen::Vector3(0.0f, 0.0f, 0.0f);
}

std::string TGen::MD3::Mesh::getDefaultMaterial() const {
	return "";
}
