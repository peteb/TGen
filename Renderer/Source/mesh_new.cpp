/*
 *  mesh_new.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/3/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "mesh_new.h"

TGen::NewMesh::NewMesh(const std::string & materialName, const std::string & materialNamePostfix)
	: TGen::NewMeshInstance(materialName, materialNamePostfix)
{
}

TGen::NewMesh::~NewMesh() {
	
}
