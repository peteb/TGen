/*
 *  mesh.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/23/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "mesh.h"

TGen::Mesh::Mesh(const std::string & name)
	: name(name)
{
}

std::string TGen::Mesh::getName() const {
	return name;
}
