/*
 *  meshdata.cpp
 *  The Place
 *
 *  Created by Peter Backman on 7/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "meshdata.h"

MeshData::MeshData(const TGen::VertexStructure & structure, uint count, void * data)
	: structure(structure), count(count), data(data)
{
		
}
