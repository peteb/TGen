/*
 *  md5mesh.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 1/23/08.
 *  Renamed by Peter Backman on 2/10/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "md5mesh.h"

TGen::MD5::Mesh::Mesh(const std::string & materialName) 
	: materialName(materialName)
{
	
}

TGen::MD5::Mesh::~Mesh() {

}

std::string TGen::MD5::Mesh::getMaterialName() const {
	return materialName;
}
